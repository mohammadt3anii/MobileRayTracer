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
    private static final String vertexShaderCode =
            "precision lowp float;" +
                    "attribute vec4 vPosition;" +
                    "attribute vec2 a_texCoord;" +
                    "varying vec2 v_texCoord;" +
                    "void main () {" +
                    "   gl_Position = vPosition;" +
                    "   v_texCoord = a_texCoord;" +
                    '}';
    private static final String fragmentShaderCode =
            "precision lowp float;" +
                    "uniform sampler2D u_Texture;" +
                    "varying vec2 v_texCoord;" +
                    "float toneMap (float value) {" +
                    //"   return 1.0 - cos(sqrt(value));" +
                    //"   return pow(1.0 - exp(-value * 1.0), 1.0 / 2.2);" +
                    "   return value;" +
                    '}' +
                    "void main () {" +
                    "   vec4 color = texture2D(u_Texture, v_texCoord);" +
                    //"   gl_FragColor = texture2D(u_Texture, v_texCoord);" +
                    "   gl_FragColor = vec4(toneMap(color[0]), toneMap(color[1]), toneMap(color[2]), 1.0);" +
                    '}';
    private final int[] textureHandle = new int[1];
    private final float[] vertices = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    private final float[] textCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };
    private int positionAttrib;
    private int mTexCoordLoc;
    private int mTextureUniformHandle;
    private int shaderProgram;
    private Bitmap bitmap;
    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;
    private GLDrawView glDrawView_;

    private static int loadShader(final int type, final String shaderCode) {
        final int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }

    void setGLDrawView(final GLDrawView gldrawView) {
        glDrawView_ = gldrawView;
    }

    @Override
    public void onDrawFrame(final GL10 arg0) {
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
        GLUtils.texSubImage2D(GLES20.GL_TEXTURE_2D, 0, 0, 0, bitmap);
    }

    @Override
    public void onSurfaceChanged(final GL10 arg0, final int arg1, final int arg2) {

    }

    @Override
    public void onSurfaceCreated(final GL10 arg0, final EGLConfig arg1) {
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

        final int vertexShader = MainRenderer.loadShader(GLES20.GL_VERTEX_SHADER, MainRenderer.vertexShaderCode);
        final int fragmentShader = MainRenderer.loadShader(GLES20.GL_FRAGMENT_SHADER, MainRenderer.fragmentShaderCode);

        shaderProgram = GLES20.glCreateProgram();

        GLES20.glAttachShader(shaderProgram, vertexShader);
        GLES20.glAttachShader(shaderProgram, fragmentShader);
        GLES20.glLinkProgram(shaderProgram);

        GLES20.glEnable(GLES20.GL_TEXTURE_2D);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

        GLES20.glGenTextures(1, textureHandle, 0);
        if (textureHandle[0] == 0) {
            Log.e("Error loading texture.", "Error loading texture");
            System.exit(0);
        } else {
            bitmap = Bitmap.createBitmap(996, 1208, Bitmap.Config.ARGB_8888);
            // Bind to the texture in OpenGL
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

            // Set filtering
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);

            GLES20.glUseProgram(shaderProgram);
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
            positionAttrib = GLES20.glGetAttribLocation(shaderProgram, "vPosition");
            GLES20.glVertexAttribPointer(positionAttrib, 3, GLES20.GL_FLOAT, false, 0, vertexBuffer);
            mTexCoordLoc = GLES20.glGetAttribLocation(shaderProgram, "a_texCoord");
            GLES20.glVertexAttribPointer(mTexCoordLoc, 2, GLES20.GL_FLOAT, false, 0, textureBuffer);
            mTextureUniformHandle = GLES20.glGetUniformLocation(shaderProgram, "u_Texture");
            GLES20.glUniform1i(mTextureUniformHandle, 0);

            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);
            GLES20.glEnableVertexAttribArray(positionAttrib);
            GLES20.glEnableVertexAttribArray(mTexCoordLoc);

            DrawView.initialize(3, 2, bitmap.getWidth(), bitmap.getHeight(), 0, 1, 1);
            DrawView.renderIntoBitmap(bitmap, 16);
        }
    }
}
