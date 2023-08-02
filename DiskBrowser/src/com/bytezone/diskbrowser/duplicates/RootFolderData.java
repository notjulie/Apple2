package com.bytezone.diskbrowser.duplicates;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

import com.bytezone.diskbrowser.gui.DuplicateAction.DiskTableSelectionListener;
import com.bytezone.diskbrowser.utilities.Utility;

// -----------------------------------------------------------------------------------//
public class RootFolderData implements PropertyChangeListener
// -----------------------------------------------------------------------------------//
{
  private static final String header =
      "      type        uncmp      .gz     .zip    total";
  private static final String line = "--------------  -------  -------  -------  -------";
  private static final Font font = new Font ("Monospaced", Font.BOLD, 15);

  private File rootFolder;
  private int rootFolderNameLength;

  final Map<Long, DiskDetails> checksumMap = new HashMap<Long, DiskDetails> ();
  final Map<String, DiskDetails> fileNameMap = new TreeMap<String, DiskDetails> ();

  public DisksWindow disksWindow;

  public final List<DiskTableSelectionListener> listeners = new ArrayList<> ();

  public boolean doChecksums;

  int totalDisks;
  int totalFolders;

  // total files for each suffix (uncompressed, .gz, .zip, total)
  int[][] typeTotals;

  // Progress dialog
  ProgressPanel progressPanel;
  public JFrame dialogTotals;
  JPanel southPanel;
  JButton btnCancel;
  JButton btnOK;

  // ---------------------------------------------------------------------------------//
  private void createWindows ()
  // ---------------------------------------------------------------------------------//
  {
    southPanel = new JPanel ();
    btnCancel = new JButton ("Cancel");
    btnOK = new JButton ("OK");

    progressPanel = new ProgressPanel ();
    progressPanel.setPreferredSize (new Dimension (560, 380));

    dialogTotals = new JFrame ("Disk Totals");
    dialogTotals.add (progressPanel, BorderLayout.CENTER);
    southPanel.add (btnCancel);               // needs to be here for the pack()
    dialogTotals.add (southPanel, BorderLayout.SOUTH);
    dialogTotals.pack ();
    dialogTotals.setLocationRelativeTo (null);

    btnCancel.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        progressPanel.cancelled = true;
        dialogTotals.setVisible (false);
      }
    });

    btnOK.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        dialogTotals.setVisible (false);
      }
    });
  }

  // ---------------------------------------------------------------------------------//
  public void count (boolean doChecksums)
  // ---------------------------------------------------------------------------------//
  {
    if (dialogTotals == null)
      createWindows ();
    clear ();

    setButton (btnCancel);

    this.doChecksums = doChecksums;
    progressPanel.cancelled = false;
    disksWindow = new DisksWindow (this);
    dialogTotals.setVisible (true);
    new DuplicateSwingWorker (this).execute ();           // start SwingWorker
  }

  // ---------------------------------------------------------------------------------//
  public void done ()                                     // SwingWorker has completed
  // ---------------------------------------------------------------------------------//
  {
    print ();
    dialogTotals.repaint ();
    dialogTotals.setVisible (false);

    if (progressPanel.cancelled)
      disksWindow = null;
    else
    {
      disksWindow.setTableData (this);
      setButton (btnOK);
    }
  }

  // ---------------------------------------------------------------------------------//
  private void setButton (JButton button)
  // ---------------------------------------------------------------------------------//
  {
    southPanel.removeAll ();
    southPanel.add (button);
    dialogTotals.revalidate ();
    dialogTotals.repaint ();
  }

  // ---------------------------------------------------------------------------------//
  String getRootFolderPathText ()
  // ---------------------------------------------------------------------------------//
  {
    String text = rootFolder.getAbsolutePath ();
    String homeDir = System.getProperty ("user.home");
    if (text.startsWith (homeDir))
      text = text.replace (homeDir, "~");
    return text;
  }

  // ---------------------------------------------------------------------------------//
  private void clear ()
  // ---------------------------------------------------------------------------------//
  {
    typeTotals = new int[4][Utility.getTotalSuffixes ()];
    totalDisks = 0;
    totalFolders = 0;

    checksumMap.clear ();
    fileNameMap.clear ();
  }

  // ---------------------------------------------------------------------------------//
  public File getRootFolder ()
  // ---------------------------------------------------------------------------------//
  {
    return rootFolder;
  }

  // ---------------------------------------------------------------------------------//
  public void incrementFolders ()
  // ---------------------------------------------------------------------------------//
  {
    ++totalFolders;
  }

  // ---------------------------------------------------------------------------------//
  public void incrementType (File file, String fileName)
  // ---------------------------------------------------------------------------------//
  {
    int pos = Utility.getSuffixNo (fileName);
    if (pos >= 0)
    {
      int cmp = fileName.endsWith (".zip") ? 2 : fileName.endsWith (".gz") ? 1 : 0;

      typeTotals[cmp][pos]++;
      typeTotals[3][pos]++;
      ++totalDisks;
    }
    else
      System.out.println ("no suffix: " + fileName);

    checkDuplicates (file, fileName);
  }

  // ---------------------------------------------------------------------------------//
  private void checkDuplicates (File file, String fileName)
  // ---------------------------------------------------------------------------------//
  {
    String rootName = file.getAbsolutePath ().substring (rootFolderNameLength);
    DiskDetails diskDetails = new DiskDetails (file, rootName, fileName, doChecksums);

    if (fileNameMap.containsKey (fileName))
      fileNameMap.get (fileName).addDuplicateName (diskDetails);
    else
      fileNameMap.put (fileName, diskDetails);

    if (doChecksums)
    {
      long checksum = diskDetails.getChecksum ();
      if (checksumMap.containsKey (checksum))
        checksumMap.get (checksum).addDuplicateChecksum (diskDetails);
      else
        checksumMap.put (checksum, diskDetails);
    }
  }

  // ---------------------------------------------------------------------------------//
  public List<DiskDetails> listDuplicates (long checksum)
  // ---------------------------------------------------------------------------------//
  {
    List<DiskDetails> list = new ArrayList<> ();
    DiskDetails original = checksumMap.get (checksum);
    if (original != null)
    {
      list.add (original);
      for (DiskDetails dd : original.getDuplicateChecksums ())
        list.add (dd);
    }

    return list;
  }

  // ---------------------------------------------------------------------------------//
  public int getTotalType (int type)
  // ---------------------------------------------------------------------------------//
  {
    return typeTotals[0][type] + typeTotals[1][type] + typeTotals[2][type];
  }

  // ---------------------------------------------------------------------------------//
  public void print ()
  // ---------------------------------------------------------------------------------//
  {
    System.out.printf ("%nFolders ...... %,7d%n", totalFolders);
    System.out.printf ("Disks ........ %,7d%n%n", totalDisks);

    int grandTotal[] = new int[4];

    System.out.println (header);
    System.out.println (line);
    for (int i = 0; i < typeTotals[0].length; i++)
    {
      System.out.printf ("%14.14s  ", Utility.getSuffix (i) + " ...........");
      for (int j = 0; j < typeTotals.length; j++)
      {
        System.out.printf ("%,7d  ", typeTotals[j][i]);
        grandTotal[j] += typeTotals[j][i];
      }
      System.out.println ();
    }

    System.out.println (line);
    System.out.printf ("Total           %,7d  %,7d  %,7d  %,7d%n%n", grandTotal[0],
        grandTotal[1], grandTotal[2], grandTotal[3]);

    if (doChecksums)
    {
      System.out.printf ("Unique checksums: %,7d%n", checksumMap.size ());
      System.out.printf ("Duplicate disks : %,7d%n", totalDisks - checksumMap.size ());
    }
  }

  // ---------------------------------------------------------------------------------//
  private void rootDirectoryChanged (File oldRootFolder, File newRootFolder)
  // ---------------------------------------------------------------------------------//
  {
    rootFolder = newRootFolder;
    rootFolderNameLength = rootFolder.getAbsolutePath ().length ();
    disksWindow = null;           // force a recount
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void propertyChange (PropertyChangeEvent evt)
  // ---------------------------------------------------------------------------------//
  {
    if (evt.getPropertyName ().equals ("RootDirectory"))
      rootDirectoryChanged ((File) evt.getOldValue (), (File) evt.getNewValue ());
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String toString ()
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder text = new StringBuilder ();

    text.append (String.format ("Root folder ....... %s%n", rootFolder));
    text.append (String.format ("Disks ............. %,d%n", totalDisks));
    text.append (String.format ("Folders ........... %,d", totalFolders));

    return text.toString ();
  }

  // ---------------------------------------------------------------------------------//
  class ProgressPanel extends JPanel
  // ---------------------------------------------------------------------------------//
  {
    public volatile boolean cancelled;

    @Override
    protected void paintComponent (Graphics graphics)
    {
      super.paintComponent (graphics);

      Graphics2D g = (Graphics2D) graphics;
      g.setColor (Color.BLACK);
      g.setFont (font);

      int x = 55;
      int y = 25;
      int lineHeight = 23;
      String line;

      g.drawString (header, x, y);
      y += lineHeight + 10;

      int grandTotal[] = new int[4];

      for (int i = 0; i < typeTotals[0].length; i++)
        if (typeTotals[3][i] > 0)
        {
          line = String.format ("%14.14s  %,7d  %,7d  %,7d  %,7d",
              Utility.getSuffix (i) + " ...........", typeTotals[0][i], typeTotals[1][i],
              typeTotals[2][i], typeTotals[3][i]);
          g.drawString (line, x, y);
          for (int j = 0; j < typeTotals.length; j++)
            grandTotal[j] += typeTotals[j][i];
          y += lineHeight;
        }

      line = String.format ("Total           %,7d  %,7d  %,7d  %,7d%n%n", grandTotal[0],
          grandTotal[1], grandTotal[2], grandTotal[3]);
      y += 10;
      g.drawString (line, x, y);

      if (doChecksums)
      {
        line = String.format ("duplicates ...                             %,7d%n",
            totalDisks - checksumMap.size ());
        y += lineHeight + 10;
        g.drawString (line, x, y);
      }
    }
  }
}