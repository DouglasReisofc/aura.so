package git.artdeell.aw4c;

import java.io.IOException;

import git.artdeell.autowax.AutoWax;
import git.artdeell.autowax.heartlist.HeartList;
import git.artdeell.autowax.iap.IapPurchase;
import git.artdeell.autowax.invitemanager.InviteManager;
import git.artdeell.autowax.spiritshop.SpiritShop;
import git.artdeell.autowax.worldquest.Spirits;

@Keep
public class CanvasMain {
    private static final AutoWax aw = new AutoWax();
    private static final String AUTH_UNAVAILABLE_MSG = "Auth indisponivel nesta versao do jogo (precisa atualizar offsets)";
    private static volatile String lastSessionFingerprint = "";
    private static volatile boolean lastSessionAvailable = false;
    private static volatile Thread activeTask;

    private static String maskUser(String userId) {
        if(userId == null || userId.length() < 8) return "desconhecido";
        return userId.substring(0, 8) + "...";
    }

    private static String summarizeToken(String token) {
        if(token == null || token.isEmpty()) return "vazio";
        int len = token.length();
        String head = token.substring(0, Math.min(6, len));
        String tail = token.substring(Math.max(0, len - 4));
        return head + "..." + tail + " (" + len + " chars)";
    }

    private static void emitSessionStatusIfChanged(boolean available, String userId, String token) {
        String currentFingerprint = available ? (userId + "|" + token).hashCode() + "" : "none";
        if (available != lastSessionAvailable || !currentFingerprint.equals(lastSessionFingerprint)) {
            if (available) {
                submitLogString("Sessão capturada: OK | user=" + maskUser(userId) + " | token=" + summarizeToken(token));
            } else {
                submitLogString("Sessão não capturada ainda. Abra o jogo e tente novamente.");
            }
            lastSessionAvailable = available;
            lastSessionFingerprint = currentFingerprint;
        }
    }

    private static void startAsyncTask(Runnable worker, Runnable unlockAction) {
        Thread t = new Thread(() -> {
            aw.resetCancel();
            activeTask = Thread.currentThread();
            try {
                worker.run();
            } finally {
                activeTask = null;
                if (unlockAction != null) unlockAction.run();
            }
        });
        t.start();
    }

    private static boolean ensureSession() {
        reauthorized();
        if (!aw.hasSession()) {
            submitLogString("Status sessão: INDISPONÍVEL");
            submitLogString(AUTH_UNAVAILABLE_MSG);
            return false;
        }
        return true;
    }

    @Keep
    public static void reauthorized() {
        String[] creds = getCredentials();
        if(creds == null || creds.length < 2) {
            aw.clearSession();
            emitSessionStatusIfChanged(false, null, null);
            return;
        }
        String user = creds[0];
        String token = creds[1];
        if(user == null || token == null || user.isEmpty() || token.isEmpty()) {
            aw.clearSession();
            emitSessionStatusIfChanged(false, null, null);
            return;
        }
        aw.resetSession(user, token);
        emitSessionStatusIfChanged(true, user, token);
    }
    @Keep
    public static void candleRun(boolean doQuests, boolean doCandles, boolean doSend, boolean doReceive, boolean fragments) {
        startAsyncTask(()->{
            if(!ensureSession()) {
                return;
            }
            if(!doQuests && !doCandles && !doSend && !doReceive && !fragments) {
                submitLogString("Selecione pelo menos uma função para executar.");
                return;
            }
            submitLogString("Iniciando execução...");
            if(fragments) aw.doRaces(false);
            if(doCandles) aw.doCandleRun();
            if(doQuests) aw.doQuests();
            if(doSend) aw.runGift();
            if(doReceive) aw.collectGifts();
        }, CanvasMain::unlockUI);
    }
    @Keep
    public static void spiritShop(byte op, long arg, float arg2) {
        switch (op) {
            case 0:
                if(!ensureSession()) {
                    CanvasMain.unlockUI();
                    return;
                }
                SpiritShop.get(aw).init();
                CanvasMain.unlockUI();
                break;
            case 1:
                SpiritShop.get(aw).pushLevel(arg, arg2);
                break;
            case 2:
                SpiritShop.get(aw).popLevel();
                break;
            case 3:
                SpiritShop.get(aw).purchase(arg);
        }
    }
    @Keep
    public static void collectLights(String lights, boolean world, boolean spirit) {
        startAsyncTask(()->{
            if(!ensureSession()) {
                return;
            }
            aw.collectLights(lights, world, spirit);
        }, CanvasMain::unlockWLCollector);
    }
    @Keep
    public static void dropLights(int count) {
        startAsyncTask(()->{
            if(!ensureSession()) {
                return;
            }
            aw.runDrop(count);
        }, CanvasMain::unlockWLCollector);
    }
    @Keep
    public static void edemRun() {
        startAsyncTask(()->{
            if(!ensureSession()) {
                return;
            }
            aw.edemRun();
        }, CanvasMain::unlockEdem);
    }

    @Keep
    public static void cancelActiveTask() {
        aw.requestCancel();
        Thread t = activeTask;
        if (t != null) t.interrupt();
        unlockUI();
        unlockWLCollector();
        unlockEdem();
        submitLogString("Operação cancelada.");
    }
    @Keep
    public static void inviteManager(byte op, int idx) {
        if(!ensureSession()) return;
        InviteManager mgr = InviteManager.get(aw);
        switch(op) {
            case 0:
                mgr.copyInvite(idx);
                break;
            case 1:
                mgr.deleteInvite(idx);
                break;
            case 2:
                mgr.reload();
                break;
        }
    }
    @Keep
    public static void createInvite(String nick) {
        if(!ensureSession()) return;
        InviteManager.get(aw).createInvite(nick);
    }
    @Keep
    public static void init(int version, boolean isBeta) {
        AutoWax.initWithParameters(version, isBeta);
        reauthorized();
    }
    @Keep
    public static void worldQuests(byte op, int idx) {
        if(!ensureSession()) return;
        switch(op) {
            case 0:
                Spirits.get(aw).load();
                break;
            case 1:
                Spirits.get(aw).executeQuest(idx);
                break;
        }
    }
    @Keep
    public static void heartSelector(byte op, int idx) {
        if(!ensureSession()) return;
        switch(op) {
            case 0:
                HeartList.get(aw).reload();
                break;
            case 1:
                HeartList.get(aw).add(idx);
                break;
            case 2:
                HeartList.get(aw).rem(idx);
                break;
        }
    }

    @Keep
    public static String[] getLevelNames() {
        try {
            if (ContextOps.hasAssets())
                return ContextOps.getAssetManager().list("Data/Levels");
        }catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    @Keep
    public static void iapReloadList(String platform) {
        if(!ensureSession()) return;
        IapPurchase.get(aw).reload(platform);
    }

    @Keep
    public static void iapBuy(String platform, String id) {
        IapPurchase.get(aw).buy(platform, id);
    }

    @Keep private static native String[] getCredentials();
    @Keep public static native void goReauthorize();
    @Keep public static native void submitLogString(String s);
    @Keep public static native void submitProgressBar(int cur, int max);
    @Keep public static native void unlockUI();
    @Keep public static native void unlockWLCollector();
    @Keep public static native void unlockEdem();
}
