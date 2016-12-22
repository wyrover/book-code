package com.example.rover.test001;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

// 日志
import com.socks.library.KLog;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        KLog.d("onCreate execute");
        KLog.d("test 1");
        KLog.d("test 2");

    }
}
