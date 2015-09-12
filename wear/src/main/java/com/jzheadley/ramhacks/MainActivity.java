package com.jzheadley.ramhacks;

import android.app.Activity;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.wearable.view.WatchViewStub;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends Activity {

    private ImageView compass;
    private float currentDegree = 0f;
    private SensorManager mSensorManager;
    private TextView tvHeading;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final WatchViewStub stub = (WatchViewStub) findViewById(R.id.watch_view_stub);
        stub.setOnLayoutInflatedListener(new WatchViewStub.OnLayoutInflatedListener() {
            @Override
            public void onLayoutInflated(WatchViewStub stub) {
                tvHeading = (TextView) stub.findViewById(R.id.);
            }
        });
    }
}
