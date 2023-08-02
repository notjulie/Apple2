package com.bytezone.diskbrowser.gui;

import java.awt.EventQueue;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.swing.Action;
import javax.swing.KeyStroke;

import com.bytezone.diskbrowser.utilities.DefaultAction;

// -----------------------------------------------------------------------------------//
class PrintAction extends DefaultAction
// -----------------------------------------------------------------------------------//
{
  OutputPanel owner;

  // ---------------------------------------------------------------------------------//
  public PrintAction (OutputPanel owner)
  // ---------------------------------------------------------------------------------//
  {
    super ("Print...", "Print the contents of the output panel",
        "/com/bytezone/diskbrowser/icons/");
    int mask = Toolkit.getDefaultToolkit ().getMenuShortcutKeyMaskEx ();
    putValue (Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke (KeyEvent.VK_P, mask));
    //    putValue (Action.MNEMONIC_KEY, KeyEvent.VK_P);

    this.owner = owner;

    setIcon (Action.SMALL_ICON, "printer_16.png");
    setIcon (Action.LARGE_ICON_KEY, "printer_32.png");
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void actionPerformed (ActionEvent e)
  // ---------------------------------------------------------------------------------//
  {
    Runnable runner = new Runnable ()
    {
      @Override
      public void run ()
      {
        try
        {
          PrinterJob job = PrinterJob.getPrinterJob ();
          job.setPrintable (new PrintDocument (owner.getCurrentText ()));
          if (job.printDialog ())
            job.print ();
        }
        catch (PrinterException e)
        {
          System.out.println ("printer error");
        }
      }
    };
    EventQueue.invokeLater (runner);
  }
}