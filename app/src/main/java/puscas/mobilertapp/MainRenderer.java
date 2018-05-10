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
    private int shaderProgram;
    private int shaderProgramRaster;
    private float[] cameraRaster = null;
    private FloatBuffer floatBufferCameraRaster_ = null;
    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewMatrix = new float[16];
    private final float[] mModelMatrix = new float[16];
    private float mWidth;
    private float mHeight;

    private void checkGLError() {
        final int glError = GLES20.glGetError();
        if (glError != GLES20.GL_NO_ERROR) {
            Log.e("glError", "glError = " + GLUtils.getEGLErrorString(glError));
            System.exit(1);
        }
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
        final int b[] = new int[realWidth_ * realHeight_];
        final int bt[] = new int[realWidth_ * realHeight_];
        final IntBuffer ib = IntBuffer.wrap(b);
        ib.position(0);

        GLES20.glReadPixels(0, 0, realWidth_, realHeight_, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, ib);
        checkGLError();

        //remember, that OpenGL bitmap is incompatible with Android bitmap
        //and so, some correction need.
        for (int i = 0, k = 0; i < realHeight_; i++, k++) {
            for (int j = 0; j < realWidth_; j++) {
                final int pixel = b[i * realWidth_ + j];
                final int blue = (pixel >> 16) & 0xff;
                final int red = (pixel << 16) & 0x00ff0000;
                final int pix1 = (pixel & 0xff00ff00) | red | blue;
                bt[(realHeight_ - k - 1) * realWidth_ + j] = pix1;
            }
        }

        final Bitmap bitmapAux = Bitmap.createBitmap(bt, realWidth_, realHeight_, Bitmap.Config.ARGB_8888);
        final Bitmap bitmap = Bitmap.createScaledBitmap(bitmapAux, width_, height_, true);
        final int pixelBefore = bitmap_.getPixel(0, 0);
        final int pixelAfter = bitmap.getPixel(0, 0);
        Log.d("MobileRT", "pixelBefore = " + pixelBefore);
        Log.d("MobileRT", "pixelAfter = " + pixelAfter);
        return bitmap;
    }

    @Override
    public void onDrawFrame(final GL10 gl) {
        // Set the camera position (View matrix)
        /*final float eyeX = cameraRaster[4];
        final float eyeY = cameraRaster[5];
        final float eyeZ = cameraRaster[6];
        final float centerX = cameraRaster[0];
        final float centerY = cameraRaster[1];
        final float centerZ = cameraRaster[2];
        final float upX = cameraRaster[12];
        final float upY = cameraRaster[13];
        final float upZ = cameraRaster[14];

        Matrix.setLookAtM(mViewMatrix, 0,
                eyeX, eyeY, eyeZ,
                centerX, centerY, centerZ,
                upX, upY, upZ
        );

        // Calculate the projection and view transformation
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);*/

        GLES20.glUseProgram(shaderProgram);
        checkGLError();


        final int positionAttrib = 0;
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, positionAttrib);
        checkGLError();
        GLES20.glEnableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib, 4, GLES20.GL_FLOAT, false, 0, floatBufferVertices_);
        checkGLError();


        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, positionAttrib, vertices.length / 4);
        checkGLError();


        GLES20.glDisableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
        checkGLError();


        GLUtils.texSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, bitmap_);
        checkGLError();
    }

    @Override
    public void onSurfaceChanged(final GL10 gl, final int width, final int height) {
        GLES20.glViewport(0, 0, width, height);
        checkGLError();

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap_, 0);
        checkGLError();

        mWidth = width;
        mHeight = height;
    }

    @Override
    public void onSurfaceCreated(final GL10 gl, final EGLConfig config) {
        cameraRaster = new float[16];

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        checkGLError();

        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        checkGLError();

        //Enable culling
        GLES20.glEnable(GLES20.GL_CULL_FACE);
        checkGLError();

        GLES20.glCullFace(GLES20.GL_BACK);
        checkGLError();

        GLES20.glFrontFace(GLES20.GL_CCW);
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

    void copyFrame(final float[] vertices, final float[] colors, final float[] camera) {
        verticesRaster = vertices;
        colorsRaster = colors;
        cameraRaster = camera;

        final ByteBuffer bbVertices2 = ByteBuffer.allocateDirect(verticesRaster.length << 2);
        bbVertices2.order(ByteOrder.nativeOrder());
        floatBufferVerticesRaster_ = bbVertices2.asFloatBuffer();
        floatBufferVerticesRaster_.put(verticesRaster);
        floatBufferVerticesRaster_.position(0);

        final ByteBuffer bbColors = ByteBuffer.allocateDirect(colorsRaster.length << 2);
        bbColors.order(ByteOrder.nativeOrder());
        floatBufferColorsRaster_ = bbColors.asFloatBuffer();
        floatBufferColorsRaster_.put(colorsRaster);
        floatBufferColorsRaster_.position(0);

        final ByteBuffer bbCamera = ByteBuffer.allocateDirect(cameraRaster.length << 2);
        bbCamera.order(ByteOrder.nativeOrder());
        floatBufferCameraRaster_ = bbCamera.asFloatBuffer();
        floatBufferCameraRaster_.put(cameraRaster);
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


        final int positionAttrib = 0;
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, positionAttrib);
        checkGLError();
        GLES20.glEnableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glVertexAttribPointer(positionAttrib, 4, GLES20.GL_FLOAT, false, 0, floatBufferVerticesRaster_);
        checkGLError();


        final float eyeX = camera[0];
        final float eyeY = camera[1];
        final float eyeZ = -camera[2];
        /*final float eyeX = 460.0f;
        final float eyeY = 500.0f;
        final float eyeZ = -1000.0f;*/
        /*final float eyeX = 0.0f;
        final float eyeY = 0.0f;
        final float eyeZ = 3.4f;*/

        final float lookX = camera[4];
        final float lookY = camera[5];
        final float lookZ = -camera[6];
        /*final float lookX = 0.0f;
        final float lookY = 400.0f;
        final float lookZ = 0.0f;*/
        /*final float lookX = 0.0f;
        final float lookY = 0.0f;
        final float lookZ = -1.0f;*/


        final float upX = camera[12];
        final float upY = camera[13];
        final float upZ = camera[14];
        /*final float upX = 0.0f;
        final float upY = 1.0f;
        final float upZ = 0.0f;*/

        final float near = 1.0f;
        final float far = 1000.0f;
        final float ratio = mWidth / mHeight;
        final float vfovFactor = mWidth < mHeight ? ratio : 1.0f;
        final float fov = 45.0f * vfovFactor;
        final float top = (float) Math.tan(fov * Math.PI / 360.0f) * near;
        //final float top = 1.0f;
        final float bottom = -top;
        final float left = ratio * bottom;
        final float right = ratio * top;

        Matrix.setIdentityM(mModelMatrix, 0);
        //Matrix.frustumM(mProjectionMatrix, 0, left, right, bottom, top, near, far);
        Matrix.perspectiveM(mProjectionMatrix, 0, fov, ratio, near, far);
        Matrix.setLookAtM(mViewMatrix, 0, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);
        Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);
        float[] verticesViewModel1 = new float[4];
        float[] verticesViewModel2 = new float[4];
        float[] verticesViewModel3 = new float[4];
        Matrix.multiplyMV(verticesViewModel1, 0, mMVPMatrix, 0, vertices, 0);
        Matrix.multiplyMV(verticesViewModel2, 0, mMVPMatrix, 0, vertices, 4);
        Matrix.multiplyMV(verticesViewModel3, 0, mMVPMatrix, 0, vertices, 8);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);
        final int handleModel = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformModelMatrix");
        final int handleView = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformViewMatrix");
        final int handleProjection = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformProjectionMatrix");
        final int handleMVP = GLES20.glGetUniformLocation(shaderProgramRaster, "uniformMVPMatrix");

        float[] verticesMVP1 = new float[4];
        float[] verticesMVP2 = new float[4];
        float[] verticesMVP3 = new float[4];
        float[] verticesView1 = new float[4];
        float[] verticesView2 = new float[4];
        float[] verticesView3 = new float[4];
        float[] verticesProjection = new float[4];
        Matrix.multiplyMV(verticesMVP1, 0, mMVPMatrix, 0, vertices, 0);
        Matrix.multiplyMV(verticesMVP2, 0, mMVPMatrix, 0, vertices, 4);
        Matrix.multiplyMV(verticesMVP3, 0, mMVPMatrix, 0, vertices, 8);
        Matrix.multiplyMV(verticesView1, 0, mViewMatrix, 0, vertices, 0);
        Matrix.multiplyMV(verticesView2, 0, mViewMatrix, 0, vertices, 4);
        Matrix.multiplyMV(verticesView3, 0, mViewMatrix, 0, vertices, 8);
        Matrix.multiplyMV(verticesProjection, 0, mProjectionMatrix, 0, vertices, 0);

        GLES20.glUniformMatrix4fv(handleModel, 1, false, mModelMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleView, 1, false, mViewMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleProjection, 1, false, mProjectionMatrix, 0);
        checkGLError();
        GLES20.glUniformMatrix4fv(handleMVP, 1, false, mMVPMatrix, 0);
        checkGLError();


        final int vertexCount = verticesRaster.length / 4;
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
        checkGLError();


        GLES20.glDisableVertexAttribArray(positionAttrib);
        checkGLError();
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, 0);
        checkGLError();


        bitmap_ = copyFrameBuffer();
    }
}
