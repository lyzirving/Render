package com.lau.render.ui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.IntDef;
import androidx.annotation.Nullable;

import com.lau.render.utils.GfxUtil;
import com.lau.render.utils.LogUtil;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class HitAABBView extends View {
    private static final String TAG = "HitAABBView";
    private static final float INVALID = -1.f;

    public static final int ACT_CLEAR      = 0;
    public static final int ACT_BOUND      = 1;
    public static final int ACT_RAY_ANCHOR = 2;
    public static final int ACT_RAY_END    = 3;
    public static final int ACT_COUNT      = 4;

    @IntDef({ACT_CLEAR, ACT_BOUND, ACT_RAY_ANCHOR, ACT_RAY_END, ACT_COUNT})
    @Retention(RetentionPolicy.SOURCE)
    public @interface ActMode {}

    private @ActMode int mMode;

    private PointF mAA, mBB;
    private PointF mStart, mEnd;
    private Paint mPaint;

    public HitAABBView(Context context) {
        this(context, null);
    }

    public HitAABBView(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public HitAABBView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mMode = ACT_BOUND;
        mAA = new PointF(INVALID, INVALID);
        mBB = new PointF(INVALID, INVALID);
        mStart = new PointF(INVALID, INVALID);
        mEnd = new PointF(INVALID, INVALID);
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mPaint.setStyle(Paint.Style.STROKE);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if(isValid(mAA) && isValid(mBB)) {
            mPaint.setColor(Color.parseColor("#FFFF0000"));
            mPaint.setStrokeWidth(10.f);
            canvas.drawRect(mAA.x, mAA.y, mBB.x, mBB.y, mPaint);
        }
        if(isValid(mAA)) {
            mPaint.setColor(Color.parseColor("#FF000000"));
            mPaint.setStrokeWidth(15.f);
            canvas.drawPoint(mAA.x, mAA.y, mPaint);
        }
        if(isValid(mBB)) {
            mPaint.setColor(Color.parseColor("#FF000000"));
            mPaint.setStrokeWidth(15.f);
            canvas.drawPoint(mBB.x, mBB.y, mPaint);
        }

        if(isValid(mStart) && isValid(mEnd)) {
            mPaint.setColor(Color.parseColor("#FF0000FF"));
            mPaint.setStrokeWidth(10.f);
            canvas.drawLine(mStart.x, mStart.y, mEnd.x, mEnd.y, mPaint);
        }

        if(isValid(mStart)) {
            mPaint.setColor(Color.parseColor("#FF00FF00"));
            mPaint.setStrokeWidth(15.f);
            canvas.drawPoint(mStart.x, mStart.y, mPaint);
        }

        if(isValid(mAA) && isValid(mBB) && isValid(mStart) && isValid(mEnd)) {
            GfxUtil.hitAABB(mStart, mEnd, mAA, mBB, getMeasuredWidth(), getMeasuredHeight());
        }

    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_UP) {
            switch (mMode) {
                case ACT_BOUND: {
                    setBound(event.getX(), event.getY());
                    invalidate();
                    break;
                }
                case ACT_RAY_ANCHOR: {
                    setAnchor(event.getX(), event.getY());
                    invalidate();
                    break;
                }
                case ACT_RAY_END: {
                    setEnd(event.getX(), event.getY());
                    invalidate();
                    break;
                }
                default: {
                    break;
                }
            }
        }
        return true;
    }

    public void setMode(@ActMode int mode) {
        if(mode >= ACT_COUNT) {
            LogUtil.d(TAG, "setMode: invalid mode = " + mode);
            return;
        }
        mMode = mode;
        if (mMode == ACT_CLEAR) {
            mAA.x = INVALID;
            mAA.y = INVALID;
            mBB.x = INVALID;
            mBB.y = INVALID;
            mStart.x = INVALID;
            mStart.y = INVALID;
            mEnd.x = INVALID;
            mEnd.y = INVALID;
            invalidate();
        }
    }

    private void setBound(float x, float y) {
        if (mAA.x < 0.f || mAA.y < 0.f) {
            mAA.x = x;
            mAA.y = y;
        } else if (mBB.x < 0.f || mBB.y < 0.f) {
            mBB.x = x;
            mBB.y = y;
        } else {
            mAA.x = x;
            mAA.y = y;
            mBB.x = INVALID;
            mBB.y = INVALID;
        }
    }

    private void setAnchor(float x, float y) {
        mStart.x = x;
        mStart.y = y;
        mEnd.x = INVALID;
        mEnd.y = INVALID;
    }

    private void setEnd(float x, float y) {
        if(mStart.x < 0.f || mStart.y < 0.f) {
            mStart.x = x;
            mStart.y = y;
        } else {
            mEnd.x = x;
            mEnd.y = y;
        }
    }

    private boolean isValid(PointF pt) {
        return pt != null && pt.x >= 0.f && pt.y >= 0.f;
    }
}
