package puscas.mobilertapp;

import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Debug;
import android.os.SystemClock;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.Callable;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

final class RenderTask extends AsyncTask<Void, Void, Void> {
    final List<TouchTracker> touches_ = new ArrayList<>(1);
    private final ScheduledExecutorService scheduler_ = Executors.newSingleThreadScheduledExecutor();
    private ViewText viewText_ = null;
    private Bitmap bitmap_ = null;
    private Callable<Integer> myFunc_ = null;
    private final Runnable timer_ = () -> {
        final int touchesSize = touches_.size();
        for (int i = 0; i < touchesSize; i++) {
            final TouchTracker touch = touches_.get(i);
            viewText_.moveTouch(touch.x_, touch.y_, touch.primitiveID_);
        }
        viewText_.FPS();
        viewText_.fpsT_ = String.format(Locale.US, "fps:%.1f", viewText_.getFPS());
        viewText_.fpsRenderT_ = String.format(Locale.US, "[%.1f]", viewText_.fps_);
        viewText_.timeFrameT_ = String.format(Locale.US, ",t:%.2fs", viewText_.getTimeFrame() / 1000.0f);
        viewText_.timeT_ = String.format(Locale.US, "[%.2fs]",
                (SystemClock.elapsedRealtime() - viewText_.start_) / 1000.0f);
        viewText_.allocatedT_ = ",m:" + Debug.getNativeHeapAllocatedSize() / 1048576L + "mb";
        viewText_.sampleT_ = "," + viewText_.getSample();
        final int stage = viewText_.isWorking();
        viewText_.stageT_ = DrawView.Stage.values()[stage].toString();
        publishProgress();
        //drawView_.requestRender();
        try {
            myFunc_.call();
        } catch (final Exception e) {
            Log.e("Error", e.getMessage());
        }
        if (stage != DrawView.Stage.busy.id_) {
            viewText_.finish(bitmap_);
            scheduler_.shutdown();
        }
    };

    RenderTask(final ViewText viewText, final Bitmap bitmap, final Callable<Integer> myFunc) {
        super();
        viewText_ = viewText;
        bitmap_ = bitmap;
        myFunc_ = myFunc;
    }

    @Override
    protected final Void doInBackground(final Void... params) {
        scheduler_.scheduleAtFixedRate(timer_, 0L, viewText_.period_, TimeUnit.MILLISECONDS);
        boolean running = true;
        do {
            try {
                running = !scheduler_.awaitTermination(2147483647L, TimeUnit.DAYS);
            } catch (final InterruptedException e) {
                e.fillInStackTrace();
            }
        } while (running);
        try {
            Thread.sleep(viewText_.period_);
        } catch (final InterruptedException e) {
            e.fillInStackTrace();
        }
        return null;
    }

    @Override
    protected final void onProgressUpdate(final Void... progress) {
        viewText_.printText();
    }

    @Override
    protected final void onPostExecute(final Void result) {
        viewText_.buttonRender_.setText(R.string.render);
        //drawView_.requestRender();
        try {
            myFunc_.call();
        } catch (final Exception e) {
            Log.e("Error", e.getMessage());
        }
    }
}
