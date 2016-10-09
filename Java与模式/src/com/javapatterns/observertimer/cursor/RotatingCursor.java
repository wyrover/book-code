package com.javapatterns.observertimer.cursor;

import javax.swing.Action;
import javax.swing.AbstractAction;
import javax.swing.Timer;

import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.applet.Applet;

public class RotatingCursor extends Applet
{
	private Action timerAction;
    private Timer timer;

    public void init()
    {
        timerAction = new TimerAction(this);
		timer = new Timer(300, timerAction);
        timer.start();
    }

}
