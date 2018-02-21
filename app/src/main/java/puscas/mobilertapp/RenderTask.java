package puscas.mobilertapp;

import android.os.AsyncTask;
import android.os.Debug;
import android.os.SystemClock;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

final class RenderTask extends AsyncTask<Void, Void, Void> {
    final List<TouchTracker> touches_ = new ArrayList<>(1);
    private final ScheduledExecutorService scheduler_ = Executors.newSingleThreadScheduledExecutor();
    private ViewText viewText_ = null;
    private Runnable updateRender_ = null;

    private final Runnable timer_ = () -> {
        final int touchesSize = touches_.size();
        for (int i = 0; i < touchesSize; i++) {
            final TouchTracker touch = touches_.get(i);
            ViewText.moveTouch(touch.x_, touch.y_, touch.primitiveID_);
        }
        viewText_.FPS();
        viewText_.fpsT_ = String.format(Locale.US, "fps:%.1f", ViewText.getFPS());
        viewText_.fpsRenderT_ = String.format(Locale.US, "[%.1f]", viewText_.fps_);
        viewText_.timeFrameT_ = String.format(Locale.US, ",t:%.2fs",
                ViewText.getTimeFrame() / 1000.0f);
        viewText_.timeT_ = String.format(Locale.US, "[%.2fs]",
                (SystemClock.elapsedRealtime() - viewText_.start_) / 1000.0f);
        viewText_.allocatedT_ = ",m:" + Debug.getNativeHeapAllocatedSize() / 1048576L + "mb";
        viewText_.sampleT_ = "," + ViewText.getSample();
        final int stage = ViewText.isWorking();
        viewText_.stageT_ = DrawView.Stage.values()[stage].toString();
        updateRender_.run();
        publishProgress();
        if (stage != DrawView.Stage.busy.id_) {
            scheduler_.shutdownNow();
        }
    };

    RenderTask(final ViewText viewText, final Runnable updateRender) {
        super();
        viewText_ = viewText;
        updateRender_ = updateRender;
    }

    @Override
    protected final Void doInBackground(final Void... params) {
        scheduler_.scheduleAtFixedRate(timer_, 0L, viewText_.period_, TimeUnit.MILLISECONDS);
        boolean running = true;
        do {
            try {
                running = !scheduler_.awaitTermination(1L, TimeUnit.DAYS);
            } catch (final InterruptedException e) {
                Log.e("InterruptedException", e.getMessage());
                System.exit(1);
            }
        } while (running);
        return null;
    }

    @Override
    protected final void onProgressUpdate(final Void... progress) {
        viewText_.printText();
    }

    @Override
    protected final void onPostExecute(final Void result) {
        viewText_.printText();
        viewText_.buttonRender_.setText(R.string.render);
        DrawView.finishRender();
        updateRender_.run();
    }
}
