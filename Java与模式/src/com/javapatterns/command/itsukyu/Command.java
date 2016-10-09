package com.javapatterns.command.itsukyu;

import java.awt.*;

abstract public class Command extends Button
{
    public Command(String caption)
    {
        super(caption);
    }

    public void execute(){}
}
