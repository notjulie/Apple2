package com.bytezone.diskbrowser.gui;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import com.bytezone.diskbrowser.utilities.DefaultAction;

// -----------------------------------------------------------------------------------//
public abstract class AbstractSaveAction extends DefaultAction
// -----------------------------------------------------------------------------------//
{
  protected JFileChooser fileChooser = new JFileChooser ();

  // ---------------------------------------------------------------------------------//
  public AbstractSaveAction (String menuText, String tip, String dialogTitle)
  // ---------------------------------------------------------------------------------//
  {
    super (menuText, tip);

    fileChooser.setDialogTitle (dialogTitle);
  }

  // ---------------------------------------------------------------------------------//
  void setSelectedFile (File file)
  // ---------------------------------------------------------------------------------//
  {
    fileChooser.setSelectedFile (file);
  }

  // ---------------------------------------------------------------------------------//
  void saveBuffer (byte[] buffer)
  // ---------------------------------------------------------------------------------//
  {
    File file = fileChooser.getSelectedFile ();
    try
    {
      Files.write (file.toPath (), buffer, StandardOpenOption.CREATE_NEW);
      JOptionPane.showMessageDialog (null, String.format ("File %s saved", file.getName ()));
    }
    catch (FileAlreadyExistsException e)
    {
      JOptionPane.showMessageDialog (null, "File " + file.getName () + " already exists", "Failed",
          JOptionPane.ERROR_MESSAGE);
    }
    catch (IOException e)
    {
      e.printStackTrace ();
      JOptionPane.showMessageDialog (null, "File failed to save - " + e.getMessage (), "Failed",
          JOptionPane.ERROR_MESSAGE);
    }
  }
}
