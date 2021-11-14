package com.glumes.demo;

import android.view.Surface;

public class Decoder {
    static {
        System.loadLibrary("native-lib");
    }
    public native void decodeToSurface(String path, Surface surface);
}
