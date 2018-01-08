package puscas.mobilertapp;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLUtils;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class MainRenderer implements Renderer {
    private final float[] vertices = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    private final float[] texCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };
    String vertexShaderCode = null;
    String fragmentShaderCode = null;
    Bitmap bitmap_ = null;

    private static int loadShader(final int shaderType, final String source) {
        final int shader = GLES20.glCreateShader(shaderType);
        if (shader != 0) {
            GLES20.glShaderSource(shader, source);
            GLES20.glCompileShader(shader);
            final int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                Log.e("SHADER", "Could not compile shader " + shaderType + ':');
                Log.e("SHADER", GLES20.glGetShaderInfoLog(shader));
                Log.e("SHADER", source);
                GLES20.glDeleteShader(shader);
                System.exit(0);
            }
        }
        return shader;
    }

    @Override
    public void onDrawFrame(final GL10 gl) {
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
        GLUtils.texSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, bitmap_);
    }

    @Override
    public void onSurfaceChanged(final GL10 gl, final int width, final int height) {
        GLES20.glViewport(0, 0, width, height);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap_, 0);
    }

    @Override
    public void onSurfaceCreated(final GL10 gl, final EGLConfig config) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        //Enable culling
        GLES20.glEnable(GLES20.GL_CULL_FACE);
        GLES20.glCullFace(GLES20.GL_BACK);
        GLES20.glFrontFace(GLES20.GL_CCW);

        //Create geometry and texCoords buffers
        final ByteBuffer bbVertices = ByteBuffer.allocateDirect(vertices.length << 2);
        bbVertices.order(ByteOrder.nativeOrder());
        final FloatBuffer floatBufferVertices = bbVertices.asFloatBuffer();
        floatBufferVertices.put(vertices);
        floatBufferVertices.position(0);

        final ByteBuffer byteBufferTexCoords = ByteBuffer.allocateDirect(texCoords.length << 2);
        byteBufferTexCoords.order(ByteOrder.nativeOrder());
        final FloatBuffer floatBufferTexture = byteBufferTexCoords.asFloatBuffer();
        floatBufferTexture.put(texCoords);
        floatBufferTexture.position(0);

        //Load shaders
        final int vertexShader = MainRenderer.loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        final int fragmentShader = MainRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        //Create Program
        final int shaderProgram = GLES20.glCreateProgram();
        if (shaderProgram == 0) {
            Log.e("PROGRAM SHADER", "Could not create program: ");
            Log.e("PROGRAM SHADER", GLES20.glGetProgramInfoLog(0));
            System.exit(0);
        }

        //Attach and link shaders to program
        GLES20.glAttachShader(shaderProgram, vertexShader);
        GLES20.glAttachShader(shaderProgram, fragmentShader);
        GLES20.glLinkProgram(shaderProgram);
        final int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(shaderProgram, GLES20.GL_LINK_STATUS, linkStatus, 0);
        if (linkStatus[0] != GLES20.GL_TRUE) {
            Log.e("PROGRAM SHADER LOG", "Could not link program: ");
            Log.e("PROGRAM SHADER LOG", GLES20.glGetProgramInfoLog(shaderProgram));
            GLES20.glDeleteProgram(shaderProgram);
            System.exit(0);
        }

        //Enable Textures
        GLES20.glEnable(GLES20.GL_TEXTURE_2D);
        final int[] textureHandle = new int[1];
        GLES20.glGenTextures(1, textureHandle, 0);
        if (textureHandle[0] == 0) {
            Log.e("Error loading texture.", "Error loading texture");
            System.exit(0);
        }

        // Bind to the texture in OpenGL
        GLES20.glUseProgram(shaderProgram);
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        //Bind Attributes
        final int positionAttrib = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
        GLES20.glVertexAttribPointer(positionAttrib, 3, GLES20.GL_FLOAT, false, 0, floatBufferVertices);
        GLES20.glEnableVertexAttribArray(positionAttrib);

        final int texCoordAttrib = GLES20.glGetAttribLocation(shaderProgram, "a_texCoord");
        GLES20.glVertexAttribPointer(texCoordAttrib, 2, GLES20.GL_FLOAT, false, 0, floatBufferTexture);
        GLES20.glEnableVertexAttribArray(texCoordAttrib);

        //Bind Uniform
        final int textureUniform = GLES20.glGetUniformLocation(shaderProgram, "u_Texture");
        GLES20.glUniform1i(textureUniform, 0);
    }
}
