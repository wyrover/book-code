package com.example.rover.test001;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

// 日志
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Log.d("HelloWorldActivity", "onCreate execute");
    }
}
