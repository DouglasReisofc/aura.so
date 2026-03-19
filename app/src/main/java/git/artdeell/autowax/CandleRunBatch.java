package git.artdeell.autowax;

import local.json.JSONObject;

public class CandleRunBatch implements Runnable{
    final AutoWax host;
    final JSONObject runBatch;
    public CandleRunBatch(AutoWax host, JSONObject runBatch) {
        this.host = host;
        this.runBatch = runBatch;
    }

    @Override
    public void run() {
        byte attempts=0;
        while(attempts < 6) {
            if (host.isCancelRequested()) return;
            try {
                JSONObject postResult = host.doPost("/account/collect_pickup_batch", runBatch);
                if (postResult.optString("result", "").equals("throttle")) {
                    attempts++;
                    Thread.sleep(Math.min(5000, postResult.optLong("cooldown", 1) * 1000L));
                    continue;
                }
                if(!postResult.isEmpty()) break;
                attempts++;
            } catch (SkyProtocolException | InterruptedException e) {
                e.printStackTrace();
                attempts++;
            }
        }
    }
}
