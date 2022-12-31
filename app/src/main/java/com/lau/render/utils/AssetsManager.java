package com.lau.render.utils;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.Objects;
import java.util.concurrent.Executors;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author lyzirving
 */
public class AssetsManager {
    private static final String TAG = "AssetsManager";
    private static volatile AssetsManager sInstance;

    private static final int END_OF_STREAM = -1;

    public enum Type {
        /**
         * resources for rendering, such as .obj file
         */
        MODEL,
        /**
         * resources for rendering, such as .png, .jpg etc.
         */
        TEXTURE,
        SHADER
    }

    private ThreadPoolExecutor mCachedThreadPool;
    private AssetsListener mListener;

    private AssetsManager() { buildThreadPoolIfNeed(); }

    public static AssetsManager get() {
        if (sInstance == null) {
            synchronized (AssetsManager.class) {
                if (sInstance == null)
                    sInstance = new AssetsManager();
            }
        }
        return sInstance;
    }

    private void buildThreadPoolIfNeed() {
        if (mCachedThreadPool == null) {
            mCachedThreadPool = new ThreadPoolExecutor(
                    0, Integer.MAX_VALUE,
                    60L, TimeUnit.SECONDS,
                    new SynchronousQueue<Runnable>(),
                    new AssetsThreadFactory(TAG),
                    new ThreadPoolExecutor.AbortPolicy());
        }
    }

    public void copyAssets(final Context ctx) {
        buildThreadPoolIfNeed();
        mCachedThreadPool.execute(new Runnable() {
            @Override
            public void run() {
                File root = ctx.getExternalFilesDir(Environment.DIRECTORY_DOCUMENTS);
                if (root != null) {
                    boolean ret = true;
                    if (!root.exists()) { ret = root.mkdirs(); }
                    if (ret) {
                        copyAssets(ctx, Type.MODEL, root.getAbsolutePath());
                        copyAssets(ctx, Type.TEXTURE, root.getAbsolutePath());
                        copyAssets(ctx, Type.SHADER, root.getAbsolutePath());
                        if (!Objects.isNull(mListener)) {
                            mListener.onCopyFinish();
                        }
                    } else {
                        LogUtil.e(TAG, "copyAssets: fail to make root directory[" + root.getAbsolutePath() + "]");
                        if (!Objects.isNull(mListener)) {
                            mListener.onCopyFail();
                        }
                    }
                } else {
                    LogUtil.e(TAG, "copyAssets: unknown err, fail to get external root");
                    if (!Objects.isNull(mListener)) {
                        mListener.onCopyFail();
                    }
                }
            }
        });
    }

    private void copyAssets(Context ctx, Type type, String dstRootDir) {
        String typeStr = getAssetsTypeStr(type);
        File dstTypeDir = new File(dstRootDir, typeStr);
        if (!dstTypeDir.exists()) { dstTypeDir.mkdirs(); }
        try {
            String[] fileNames = ctx.getAssets().list(typeStr);
            if (fileNames != null && fileNames.length > 0)
                copyAssets(ctx, typeStr, fileNames, type, dstRootDir + "/" + typeStr);
            else
                LogUtil.e(TAG, "copyAssets: failed to list assets[" + typeStr + "]");
        } catch (Exception e) {
            LogUtil.d(TAG, "copyAssets: failed to get assets[" + typeStr + "], exception = " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void copyAssets(Context ctx, String assetsRoot, String[] assetsNames, Type type, String dstDirectory) {
        for (String name : assetsNames) {
            if (isAssetsFile(type, name)) {
                copyFile(ctx, assetsRoot, name, dstDirectory);
            } else if (name.contains(".")) {
                LogUtil.e(TAG, "copyAssets: unknown type for name[" + assetsRoot + "/" + name + "], skip this");
            } else {// directory
                String path = assetsRoot + "/" + name;
                String dstPath = dstDirectory + "/" + name;
                File dstPathFile = new File(dstPath);
                if (!dstPathFile.exists()) { dstPathFile.mkdirs(); }
                try {
                    String[] subFileNames = ctx.getAssets().list(path);
                    if (subFileNames != null && subFileNames.length > 0)
                        copyAssets(ctx, path, subFileNames, type, dstPath);
                    else
                        LogUtil.e(TAG, "copyAssets: failed to list assets in assets path[" + path + "]");
                } catch (Exception e) {
                    LogUtil.i(TAG, "copyAssets: failed to get assets in assets path[" + path + "], exception = " + e.getMessage());
                    e.printStackTrace();
                }
            }
        }
    }

    private void copyFile(Context ctx, String assetsRoot, String name, String dstDirectory) {
        File dst = new File(dstDirectory, name);
        if (!dst.exists()) {
            String assetsFileDir = assetsRoot + "/" + name;
            try {
                InputStream is = ctx.getAssets().open(assetsFileDir);
                FileOutputStream fos = new FileOutputStream(dst);
                byte[] buffer = new byte[1024];
                int byteCount;
                while ((byteCount = is.read(buffer)) != END_OF_STREAM) {
                    fos.write(buffer, 0, byteCount);
                }
                fos.flush();
                is.close();
                fos.close();
            } catch (Exception e) {
                LogUtil.e(TAG, "copyFile: exception happens for assets[" + assetsFileDir + "]"
                             + ", dst[" + dst.getAbsolutePath() + "], message = " + e.getMessage());
                e.printStackTrace();
            }
        }
    }

    public void destroy() {
        if (mCachedThreadPool != null) {
            mCachedThreadPool.shutdown();
            mCachedThreadPool = null;
        }
        mListener = null;
    }

    public void setListener(AssetsListener listener) {
        mListener = listener;
    }

    private String getAssetsTypeStr(Type type) {
        switch (type) {
            case MODEL:
                return "model";
            case TEXTURE:
                return "texture";
            case SHADER:
                return "shader";
            default:
                return "";
        }
    }

    private boolean isAssetsFile(Type type, String name) {
        switch (type) {
            case MODEL:
                return name.contains(".obj") || name.contains(".png") || name.contains(".mtl")
                        || name.contains(".blend") || name.contains(".txt");
            case TEXTURE:
                return name.contains(".png") || name.contains(".jpg");
            case SHADER:
                return name.contains(".vert") || name.contains(".frag");
            default:
                return false;
        }
    }

    private static class AssetsThreadFactory implements ThreadFactory {
        private final ThreadFactory mDefaultThreadFactory;
        private final String mBaseName;
        private final AtomicInteger mCount = new AtomicInteger(0);

        public AssetsThreadFactory(final String baseName) {
            mDefaultThreadFactory = Executors.defaultThreadFactory();
            mBaseName = baseName;
        }

        @Override
        public Thread newThread(Runnable r) {
            final Thread thread = mDefaultThreadFactory.newThread(r);
            thread.setName(mBaseName + "-" + mCount.getAndIncrement());
            return thread;
        }
    }

    public static interface AssetsListener {
        void onCopyFinish();
        void onCopyFail();
    }
}
