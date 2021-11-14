package com.glumes.demo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

import java.io.File;

public class DecodeSurfaceActivity extends AppCompatActivity {

    private SurfaceView surfaceView;
    private Button button;
    private Decoder decoder;

    private String path = Environment.getExternalStorageDirectory() + File.separator + "resource/sintel.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_decode_surface);
        surfaceView = findViewById(R.id.surface);
        button = findViewById(R.id.button);
        decoder = new Decoder();
        surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {

            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                decoder.decodeToSurface(path,surfaceView.getHolder().getSurface());
            }
        });
    }
}