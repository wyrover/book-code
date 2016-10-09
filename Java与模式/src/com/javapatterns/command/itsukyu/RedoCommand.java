package com.javapatterns.command.itsukyu;

import javax.swing.*;

public class RedoCommand extends Command {
    UndoableTextArea text;

    public RedoCommand(UndoableTextArea text)
    {
        super("Redo");
        this.text = text;
    }

    public void execute()
    {
        text.redo();
    }
}
