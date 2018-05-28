package puscas.mobilertapp;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainRenderer implements Renderer {
    private final float[] vertices = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
    };
    private final float[] texCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };
    private float[] verticesRaster = null;
    private float[] colorsRaster = null;
    private FloatBuffer floatBufferVertices_ = null;
    private FloatBuffer floatBufferTexture_ = null;
    String vertexShaderCode = null;
    String fragmentShaderCode = null;
    private int width_ = 1;
    private int height_ = 1;
    private int realWidth_ = 1;
    private int realHeight_ = 1;
    String vertexShaderCodeRaster = null;
    String fragmentShaderCodeRaster = null;
    Bitmap bitmap_ = null;
    private FloatBuffer floatBufferVerticesRaster_ = null;
    private FloatBuffer floatBufferColorsRaster_ = null;
    private FloatBuffer floatBufferCameraRaster_ = null;
    private int shaderProgram;
    private int shaderProgramRaster;
    private float[] cameraRaster = null;
    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewMatrix = new float[16];
    private final float[] mModelMatrix = new float[16];

    private void checkGLError() {
        final int glError = GLES20.glGetError();
        String stringError = null;
        switch (glError) {
            case GLES20.GL_NO_ERROR:
                return;

            case GLES20.GL_INVALID_ENUM:
                stringError = "GL_INVALID_ENUM";
                break;

            case GLES20.GL_INVALID_VALUE:
                stringError = "GL_INVALID_VALUE";
                break;

            case GLES20.GL_INVALID_OPERATION:
                stringError = "GL_INVALID_OPERATION";
                break;

            case GLES20.GL_OUT_OF_MEMORY:
                stringError = "GL_OUT_OF_MEMORY";
                break;
        }
        Log.e("glError", "glError = " + GLUtils.getEGLErrorString(glError) + ": " + stringError);
        System.exit(1);
    }

    private int loadShader(final int shaderType, final String source) {
        final int shader = GLES20.glCreateShader(shaderType);
        checkGLError();
        if (shader != 0) {
            GLES20.glShaderSource(shader, source);
            checkGLError();
            GLES20.glCompileShader(shader);
            checkGLError();
            final int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            checkGLError();
            if (compiled[0] == 0) {
                Log.e("SHADER", "Could not compile shader " + shaderType + ':');
                Log.e("SHADER", GLES20.glGetShaderInfoLog(shader));
                checkGLError();
                Log.e("SHADER", source);
                GLES20.glDeleteShader(shader);
                checkGLError();
                System.exit(1);
            }
        } else {
            Log.e("MobileRT", "shader = 0");
            System.exit(1);
        }
        return shader;
    }

    public void setBitmap(final int width, final int height, final int realWidth, final int realHeight) {
        bitmap_ = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        bitmap_.eraseColor(Color.BLACK);
        width_ = width;
        height_ = height;
        realWidth_ = realWidth;
        realHeight_ = realHeight;
    }

    private Bitmap copyFrameBuffer() {
        final int sizePixels = realWidth_ * realHeight_;
        final int b[] = new int[sizePixels];
        final int bt[] = new int[sizePixels];
        final IntBuffer ib = IntBuffer.wrap(b);
        ib.position(0);

        GLES20.glReadPixels(0, 0, realWidth_, realHeight_, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, ib);
        checkGLError();

        //remember, that OpenGL bitmap is incompatible with Android bitmap
        //and so, some correction need.
        for (int i = 0; i < realHeight_; i++) {
            for (int j = 0; j < realWidth_; j++) {
                final int oldPixelId = i * realWidth_ + j;
                final int newPixelId = (realHeight_ - i - 1) * realWidth_ + j;
                final int pixel = b[oldPixelId];
                final int red = pixel & 0xff;
                final int green = (pixel >> 8) & 0xff;
                final int blue = (pixel >> 16) & 0xff;
                final int alpha = (pixel >> 24) & 0xff;
                final int newPixel = (red << 16) | (green << 8) | blue | (alpha << 24);
                bt[newPixelId] = newPixel;
            }
        }

        final Bitmap bitmapAux = Bitmap.createBitmap(bt, realWidth_, realHeight_, Bitmap.Config.ARGB_8888);
        final Bitmap bitmap = Bitmap.createScaledBitmap(bitmapAux, width_, height_, true);
        return bitmap;
    }

    @Override
    public void onDrawFrame(final GL10 gl) {
        GLES20.glUseProgram(shaderProgram);
        checkGLError();


        final int positionAttrib = GLES20.glGetAttribLocation(shaderProgram, "vertexPosition");
        checkGLError();
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, positionAttrib);
        checkGLError();
        GLES20.glEnableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib, 4, GLES20.GL_FLOAT, false, 0, floatBufferVertices_);
        checkGLError();


        final int vertexCount = vertices.length / (Float.SIZE / Byte.SIZE);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, positionAttrib, vertexCount);
        checkGLError();


        GLES20.glDisableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
        checkGLError();

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, bitmap_, GLES20.GL_UNSIGNED_BYTE, 0);
        checkGLError();
    }

    @Override
    public void onSurfaceChanged(final GL10 gl, final int width, final int height) {
        GLES20.glViewport(0, 0, width, height);
        checkGLError();

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap_, 0);
        checkGLError();
    }

    @Override
    public void onSurfaceCreated(final GL10 gl, final EGLConfig config) {
        cameraRaster = new float[16];

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        checkGLError();

        //Enable culling
        GLES20.glEnable(GLES20.GL_CULL_FACE);
        checkGLError();

        GLES20.glEnable(GLES20.GL_BLEND);
        checkGLError();

        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
        checkGLError();

        GLES20.glCullFace(GLES20.GL_BACK);
        checkGLError();

        GLES20.glFrontFace(GLES20.GL_CCW);
        checkGLError();

        GLES20.glClearDepthf(1.0f);
        checkGLError();

        GLES20.glDepthMask(true);
        checkGLError();

        GLES20.glDepthFunc(GLES20.GL_LEQUAL);
        checkGLError();

        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        checkGLError();

        //Create geometry and texCoords buffers
        final ByteBuffer bbVertices = ByteBuffer.allocateDirect(vertices.length << 2);
        bbVertices.order(ByteOrder.nativeOrder());
        floatBufferVertices_ = bbVertices.asFloatBuffer();
        floatBufferVertices_.put(vertices);
        floatBufferVertices_.position(0);

        final ByteBuffer byteBufferTexCoords = ByteBuffer.allocateDirect(texCoords.length << 2);
        byteBufferTexCoords.order(ByteOrder.nativeOrder());
        floatBufferTexture_ = byteBufferTexCoords.asFloatBuffer();
        floatBufferTexture_.put(texCoords);
        floatBufferTexture_.position(0);


        //Load shaders
        final int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        final int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        //Create Program
        shaderProgram = GLES20.glCreateProgram();
        checkGLError();

        if (shaderProgram == 0) {
            Log.e("PROGRAM SHADER", "Could not create program: ");
            Log.e("PROGRAM SHADER", GLES20.glGetProgramInfoLog(0));
            System.exit(0);
        }

        //Attach and link shaders to program
        GLES20.glAttachShader(shaderProgram, vertexShader);
        checkGLError();

        GLES20.glAttachShader(shaderProgram, fragmentShader);
        checkGLError();


        final int Number_Texures = 1;
        final int[] textureHandle = new int[Number_Texures];
        GLES20.glGenTextures(Number_Texures, textureHandle, 0);
        if (textureHandle[0] == 0) {
            Log.e("Error loading texture.", "Error loading texture");
            System.exit(0);
        }


        //Bind Attributes
        final int positionAttrib = 0;
        GLES20.glBindAttribLocation(shaderProgram, positionAttrib, "vertexPosition");
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib, 4, GLES20.GL_FLOAT, false, 0, floatBufferVertices_);
        checkGLError();
        GLES20.glEnableVertexAttribArray(positionAttrib);
        checkGLError();

        final int texCoordAttrib = 1;
        GLES20.glBindAttribLocation(shaderProgram, texCoordAttrib, "vertexTexCoord");
        checkGLError();
        GLES20.glVertexAttribPointer(texCoordAttrib, 2, GLES20.GL_FLOAT, false, 0, floatBufferTexture_);
        checkGLError();
        GLES20.glEnableVertexAttribArray(texCoordAttrib);
        checkGLError();


        GLES20.glLinkProgram(shaderProgram);
        checkGLError();

        final int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(shaderProgram, GLES20.GL_LINK_STATUS, linkStatus, 0);
        checkGLError();

        if (linkStatus[0] != GLES20.GL_TRUE) {
            Log.e("PROGRAM SHADER LOG", "Could not link program: ");
            Log.e("PROGRAM SHADER LOG", GLES20.glGetProgramInfoLog(shaderProgram));
            GLES20.glDeleteProgram(shaderProgram);
            System.exit(0);
        }

        //Shader program 1
        GLES20.glUseProgram(shaderProgram);
        checkGLError();

        // Bind to the texture in OpenGL
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        checkGLError();

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);
        checkGLError();

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        checkGLError();

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        checkGLError();
    }

    void copyFrame(final ByteBuffer bbVertices, final ByteBuffer bbColors, final ByteBuffer bbCamera) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        checkGLError();

        bbVertices.order(ByteOrder.nativeOrder());
        floatBufferVerticesRaster_ = bbVertices.asFloatBuffer();
        floatBufferVerticesRaster_.position(0);

        bbColors.order(ByteOrder.nativeOrder());
        floatBufferColorsRaster_ = bbColors.asFloatBuffer();
        floatBufferColorsRaster_.position(0);

        bbCamera.order(ByteOrder.nativeOrder());
        floatBufferCameraRaster_ = bbCamera.asFloatBuffer();
        floatBufferCameraRaster_.position(0);

        //Load shaders
        final int vertexShaderRaster = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCodeRaster);
        final int fragmentShaderRaster = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCodeRaster);

        //Create Program
        if (shaderProgramRaster != 0) {
            GLES20.glDeleteProgram(shaderProgramRaster);
            checkGLError();
            shaderProgramRaster = 0;
        }
        shaderProgramRaster = GLES20.glCreateProgram();
        checkGLError();

        final int positionAttrib2 = 2;
        GLES20.glBindAttribLocation(shaderProgramRaster, positionAttrib2, "vertexPosition");
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib2, 4, GLES20.GL_FLOAT, false, 0, floatBufferVerticesRaster_);
        checkGLError();
        GLES20.glEnableVertexAttribArray(positionAttrib2);
        checkGLError();

        final int colorAttrib2 = 3;
        GLES20.glBindAttribLocation(shaderProgramRaster, colorAttrib2, "vertexColor");
        checkGLError();
        GLES20.glVertexAttribPointer(colorAttrib2, 4, GLES20.GL_FLOAT, false, 0, floatBufferColorsRaster_);
        checkGLError();
        GLES20.glEnableVertexAttribArray(colorAttrib2);
        checkGLError();

        //Attach and link shaders to program
        GLES20.glAttachShader(shaderProgramRaster, vertexShaderRaster);
        checkGLError();

        GLES20.glAttachShader(shaderProgramRaster, fragmentShaderRaster);
        checkGLError();

        GLES20.glLinkProgram(shaderProgramRaster);
        checkGLError();

        final int[] linkStatusRaster = new int[1];
        GLES20.glGetProgramiv(shaderProgramRaster, GLES20.GL_LINK_STATUS, linkStatusRaster, 0);
        checkGLError();

        if (linkStatusRaster[0] != GLES20.GL_TRUE) {
            Log.e("PROGRAM SHADER LOG", "Could not link program rasterizer: ");
            Log.e("PROGRAM SHADER LOG", GLES20.glGetProgramInfoLog(shaderProgramRaster));
            checkGLError();
            GLES20.glDeleteProgram(shaderProgramRaster);
            checkGLError();
            System.exit(0);
        }


        GLES20.glUseProgram(shaderProgramRaster);
        checkGLError();


        final float zNear = 0.1f;
        final float zFar = 1.0e+30f;
        final float ratio = Math.max(width_ / height_, height_ / width_);
        final float vfovFactor = width_ < height_ ? ratio : 1.0f;
        final float fovy = 45.0f * vfovFactor;

        final float eyeX = floatBufferCameraRaster_.get(0);
        final float eyeY = floatBufferCameraRaster_.get(1);
        final float eyeZ = -floatBufferCameraRaster_.get(2);
        final float dirX = floatBufferCameraRaster_.get(4);
        final float dirY = floatBufferCameraRaster_.get(5);
        final float dirZ = -floatBufferCameraRaster_.get(6);
        final float upX = floatBufferCameraRaster_.get(8);
        final float upY = floatBufferCameraRaster_.get(9);
        final float upZ = -floatBufferCameraRaster_.get(10);
        final float centerX = eyeX + dirX;
        final float centerY = eyeY + dirY;
        final float centerZ = eyeZ + dirZ;

        Matrix.setIdentityM(mModelMatrix, 0);
        Matrix.perspectiveM(mProjectionMatrix, 0, fovy, ratio, zNear, zFar);
        Matrix.setLookAtM(mViewMatrix, 0, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
        final int handleModel = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformModelMatrix");
        final int handleView = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformViewMatrix");
        final int handleProjection = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformProjectionMatrix");
        final int handleMVP = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformMVPMatrix");

        GLES20.glUniformMatrix4fv(handleModel, 1, false, mModelMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleView, 1, false, mViewMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleProjection, 1, false, mProjectionMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleMVP, 1, false, mMVPMatrix, 0);
        checkGLError();


        final int vertexCount = floatBufferVerticesRaster_.capacity() / (Float.SIZE / Byte.SIZE);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
        checkGLError();


        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
        checkGLError();


        bitmap_ = copyFrameBuffer();

        floatBufferVerticesRaster_ = null;
        floatBufferColorsRaster_ = null;

        verticesRaster = null;
        colorsRaster = null;
    }
}
