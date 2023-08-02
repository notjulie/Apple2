package com.bytezone.diskbrowser.gui;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.File;
import java.util.prefs.Preferences;

import javax.swing.Action;
import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import com.bytezone.diskbrowser.utilities.DefaultAction;

// -----------------------------------------------------------------------------------//
class RootDirectoryAction extends DefaultAction implements QuitListener
// -----------------------------------------------------------------------------------//
{
  private static final String prefsRootDirectory = "Root directory";

  private File rootFolder;

  // ---------------------------------------------------------------------------------//
  RootDirectoryAction ()
  // ---------------------------------------------------------------------------------//
  {
    super ("Set HOME folder...", "Defines root folder where the disk images are kept",
        "/com/bytezone/diskbrowser/icons/");

    putValue (Action.ACCELERATOR_KEY, KeyStroke.getKeyStroke ("alt H"));
    putValue (Action.MNEMONIC_KEY, KeyEvent.VK_H);

    setIcon (Action.SMALL_ICON, "folder_explore_16.png");
    setIcon (Action.LARGE_ICON_KEY, "folder_explore_32.png");
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void actionPerformed (ActionEvent e)
  // ---------------------------------------------------------------------------------//
  {
    JFileChooser chooser = new JFileChooser (System.getProperty ("user.home"));
    chooser.setDialogTitle ("Select FOLDER containing disk images");
    chooser.setFileSelectionMode (JFileChooser.DIRECTORIES_ONLY);
    if (rootFolder != null)
      chooser.setSelectedFile (rootFolder);

    int result = chooser.showDialog (null, "Accept");
    if (result == JFileChooser.APPROVE_OPTION)
    {
      File rootDirectoryFile = chooser.getSelectedFile ();
      if (!rootDirectoryFile.isDirectory ())
        rootDirectoryFile = rootDirectoryFile.getParentFile ();
      if (rootDirectoryFile != null)
        notifyListeners (rootDirectoryFile);
    }
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void quit (Preferences prefs)
  // ---------------------------------------------------------------------------------//
  {
    prefs.put (prefsRootDirectory,
        rootFolder == null ? "" : rootFolder.getAbsolutePath ());
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void restore (Preferences prefs)
  // ---------------------------------------------------------------------------------//
  {
    File rootDirectoryFile = new File (prefs.get (prefsRootDirectory, ""));

    if (!rootDirectoryFile.exists () || !rootDirectoryFile.isDirectory ())
    {
      System.out.println ("No root directory");
      return;
    }
    notifyListeners (rootDirectoryFile);
  }

  // ---------------------------------------------------------------------------------//
  private void notifyListeners (File newRootFolder)
  // ---------------------------------------------------------------------------------//
  {
    File oldRootFolder = rootFolder;
    rootFolder = newRootFolder;

    firePropertyChange ("RootDirectory", oldRootFolder, newRootFolder);
  }
}