package puscas.mobilertapp;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class Triangle {
    private static final String vertexShaderCode = "attribute vec4 vPosition;" +
            "attribute vec2 a_texCoord;" +
            "varying vec2 v_texCoord;" +
            "void main () {" +
            "   gl_Position = vPosition;" +
            "   v_texCoord = a_texCoord;" +
            '}';
    private static final String fragmentShaderCode = "precision mediump float;" +
            "uniform vec4 vColor;" +
            "uniform sampler2D u_Texture;" +
            "varying vec2 v_texCoord;" +
            "void main () {" +
            //"gl_FragColor = (v_Color texture2D(u_Texture, v_TexCoordinate));" +
            //"   gl_FragColor = vec4(vColor.r * v_TexCoordinate.u, vColor.g * v_TexCoordinate.u, vColor.b * v_TexCoordinate.u, vColor.a * v_TexCoordinate.u);" +
            //"   gl_FragColor = vec4(v_texCoord[0]*v_texCoord[1],v_texCoord[0]*v_texCoord[1],v_texCoord[0]*v_texCoord[1], 0.0);" +
            "   gl_FragColor = texture2D(u_Texture, v_texCoord);" +
            '}';
    private final FloatBuffer vertexBuffer;
    private final FloatBuffer textureBuffer;
    private final int[] textureHandle = new int[1];
    private final float[] vertices = {
            -0.9f, 0.9f, 0.0f,
            -0.9f, -0.9f, 0.0f,
            0.9f, -0.9f, 0.0f,
            0.9f, 0.9f, 0.0f
    };
    private final float[] textCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };
    private final float[] color = {0.0f, 0.6f, 1.0f, 1.0f};
    private final int shaderProgram;
    //private int textureHandle;

    Triangle() {
        super();
        final ByteBuffer bb1 = ByteBuffer.allocateDirect(vertices.length << 2);
        bb1.order(ByteOrder.nativeOrder());
        vertexBuffer = bb1.asFloatBuffer();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);

        final ByteBuffer bb2 = ByteBuffer.allocateDirect(textCoords.length << 2);
        bb2.order(ByteOrder.nativeOrder());
        textureBuffer = bb2.asFloatBuffer();
        textureBuffer.put(textCoords);
        textureBuffer.position(0);

        final int vertexShader = Triangle.loadShader(GLES20.GL_VERTEX_SHADER, Triangle.vertexShaderCode);
        final int fragmentShader = Triangle.loadShader(GLES20.GL_FRAGMENT_SHADER, Triangle.fragmentShaderCode);

        shaderProgram = GLES20.glCreateProgram();
        Triangle.checkGLError("glCreateProgram");

        GLES20.glAttachShader(shaderProgram, vertexShader);
        Triangle.checkGLError("glAttachShader");
        GLES20.glAttachShader(shaderProgram, fragmentShader);
        Triangle.checkGLError("glAttachShader");
        GLES20.glLinkProgram(shaderProgram);
        Triangle.checkGLError("glLinkProgram");

        GLES20.glEnable(GLES20.GL_TEXTURE_2D);
        Triangle.checkGLError("glEnable");
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        GLES20.glGenTextures(1, textureHandle, 0);
        if (textureHandle[0] != 0) {
            // Read in the resource
            final Bitmap bitmap = Bitmap.createBitmap(500, 500, Bitmap.Config.ARGB_8888);
            bitmap.eraseColor(Color.GREEN);
            final int[] pixels = new int[bitmap.getHeight() * bitmap.getWidth()];
            bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
            final int num = bitmap.getWidth() * 50;
            for (int i = 0; i < num; i++) {
                pixels[i] = Color.BLUE;
            }
            bitmap.setPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
            // Bind to the texture in OpenGL
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

            // Set filtering
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);

            // Load the bitmap into the bound texture.
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

            // Recycle the bitmap, since its data has been loaded into OpenGL.
            bitmap.recycle();
        }

        if (textureHandle[0] == 0) {
            Log.e("Error loading texture.", "Error loading texture");
        }

    }

    private static int loadShader(final int type, final String shaderCode) {
        final int shader = GLES20.glCreateShader(type);
        Triangle.checkGLError("glCreateShader");
        GLES20.glShaderSource(shader, shaderCode);
        Triangle.checkGLError("glShaderSource");
        GLES20.glCompileShader(shader);
        Triangle.checkGLError("glShaderSource");
        return shader;
    }

    private static void checkGLError(final String op) {
        int error = GLES20.glGetError();
        while (error != GLES20.GL_NO_ERROR) {
            Log.e("MyApp OPENGL ----> ", op + ": glError " + error);
            error = GLES20.glGetError();
        }
    }

    void draw() {
        GLES20.glUseProgram(shaderProgram);
        Triangle.checkGLError("glUseProgram");

        final int positionAttrib = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
        Triangle.checkGLError("glGetAttribLocation");
        GLES20.glEnableVertexAttribArray(positionAttrib);
        Triangle.checkGLError("glGetAttribLocation");
        GLES20.glVertexAttribPointer(positionAttrib, 3, GLES20.GL_FLOAT, false, 0, vertexBuffer);
        Triangle.checkGLError("glGetAttribLocation");

        final int colorUniform = GLES20.glGetUniformLocation(shaderProgram, "vColor");
        Triangle.checkGLError("glGetAttribLocation");
        GLES20.glUniform4fv(colorUniform, 1, color, 0);
        Triangle.checkGLError("glGetAttribLocation");

        // Get handle to texture coordinates location
        final int mTexCoordLoc = GLES20.glGetAttribLocation(shaderProgram, "a_texCoord");
        Triangle.checkGLError("glGetAttribLocation");
        // Enable generic vertex attribute array
        GLES20.glEnableVertexAttribArray(mTexCoordLoc);
        Triangle.checkGLError("glGetAttribLocation");
        // Prepare the texturecoordinates
        GLES20.glVertexAttribPointer(mTexCoordLoc, 2, GLES20.GL_FLOAT, false, 0, textureBuffer);
        Triangle.checkGLError("glGetAttribLocation");


        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, vertices.length / 3);
        Triangle.checkGLError("glGetAttribLocation");
        GLES20.glDisableVertexAttribArray(positionAttrib);
        Triangle.checkGLError("glGetAttribLocation");


        // Set the active texture unit to texture unit 0.
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        final int mTextureUniformHandle = GLES20.glGetUniformLocation(shaderProgram, "u_Texture");
        // Bind the texture to this unit.
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

        // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
        GLES20.glUniform1i(mTextureUniformHandle, 0);
    }
}
