package com.glumes.demo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import com.tbruyelle.rxpermissions2.RxPermissions;

import java.io.File;


public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    private RxPermissions rxPermissions;

    private String path = Environment.getExternalStorageDirectory() + File.separator + "resource/sintel.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        Button tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        rxPermissions = new RxPermissions(this);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                rxPermissions.request(
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,
                        Manifest.permission.READ_EXTERNAL_STORAGE
                ).subscribe();
                decodeWithPath(path);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void decodeWithPath(String path);
}
