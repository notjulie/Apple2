package com.bytezone.diskbrowser.gui;

import static javax.swing.ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS;
import static javax.swing.ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import com.bytezone.diskbrowser.disk.Disk;
import com.bytezone.diskbrowser.disk.DiskAddress;
import com.bytezone.diskbrowser.disk.FormattedDisk;
import com.bytezone.diskbrowser.disk.HybridDisk;
import com.bytezone.diskbrowser.gui.RedoHandler.RedoEvent;
import com.bytezone.diskbrowser.gui.RedoHandler.RedoListener;

// -----------------------------------------------------------------------------------//
class DiskLayoutPanel extends JPanel implements DiskSelectionListener,
    FileSelectionListener, RedoListener, PropertyChangeListener
// -----------------------------------------------------------------------------------//
{
  private static final int SIZE = 15;             // basic unit of a display block

  private final DiskLayoutImage diskLayoutImage;
  private final ScrollRuler verticalRuler;
  private final ScrollRuler horizontalRuler;
  private final DiskLegendPanel legendPanel;
  private final JScrollPane sp;
  private LayoutDetails layout;

  // ---------------------------------------------------------------------------------//
  public DiskLayoutPanel ()
  // ---------------------------------------------------------------------------------//
  {
    super (new BorderLayout ());

    diskLayoutImage = new DiskLayoutImage ();
    verticalRuler = new ScrollRuler (diskLayoutImage, ScrollRuler.VERTICAL);
    horizontalRuler = new ScrollRuler (diskLayoutImage, ScrollRuler.HORIZONTAL);
    legendPanel = new DiskLegendPanel ();

    setBackground (Color.WHITE);

    sp = new JScrollPane (diskLayoutImage, VERTICAL_SCROLLBAR_ALWAYS,
        HORIZONTAL_SCROLLBAR_ALWAYS);
    sp.getViewport ().setBackground (Color.WHITE);
    sp.setColumnHeaderView (horizontalRuler);
    sp.setRowHeaderView (verticalRuler);
    sp.setBorder (null);
    sp.setCorner (JScrollPane.UPPER_LEFT_CORNER, new Corner (true));
    sp.setCorner (JScrollPane.LOWER_LEFT_CORNER, new Corner (false));
    sp.setCorner (JScrollPane.UPPER_RIGHT_CORNER, new Corner (false));
    sp.setCorner (JScrollPane.LOWER_RIGHT_CORNER, new Corner (false));

    // this is just so the pack is correct
    add (sp, BorderLayout.CENTER);
    add (legendPanel, BorderLayout.SOUTH);
  }

  // ---------------------------------------------------------------------------------//
  public void setDisk (final FormattedDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    layout = new LayoutDetails (disk);
    diskLayoutImage.setDisk (disk, layout);
    verticalRuler.setLayout (layout);
    horizontalRuler.setLayout (layout);
    legendPanel.setDisk (disk, layout);

    // this is the only way I know of to force a refresh
    sp.setViewportView (diskLayoutImage);

    setLayout (new BorderLayout ());
    if (disk.getGridLayout ().height == 35)       // what about 48?
    {
      add (sp, BorderLayout.NORTH);
      add (legendPanel, BorderLayout.CENTER);
    }
    else
    {
      add (sp, BorderLayout.CENTER);
      add (legendPanel, BorderLayout.SOUTH);
    }

    // Allow the disk to notify us if the interleave or blocksize is changed
    disk.getDisk ().addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        LayoutDetails layout = new LayoutDetails (disk);

        diskLayoutImage.setDisk (disk, layout);
        legendPanel.setDisk (disk, layout);

        verticalRuler.setLayout (layout);
        horizontalRuler.setLayout (layout);
      }
    });

    repaint ();
  }

  // ---------------------------------------------------------------------------------//
  public void setHex (boolean hex)
  // ---------------------------------------------------------------------------------//
  {
    verticalRuler.setHex (hex);
    horizontalRuler.setHex (hex);
  }

  // ---------------------------------------------------------------------------------//
  public void setBlock (boolean block)
  // ---------------------------------------------------------------------------------//
  {
    verticalRuler.setTrackMode (block);
    horizontalRuler.setTrackMode (block);
  }

  // ---------------------------------------------------------------------------------//
  public void setFree (boolean free)
  // ---------------------------------------------------------------------------------//
  {
    diskLayoutImage.setShowFreeSectors (free);
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void propertyChange (PropertyChangeEvent evt)
  // ---------------------------------------------------------------------------------//
  {
    setFree ((Boolean) evt.getNewValue ());
  }

  // ---------------------------------------------------------------------------------//
  public void addSectorSelectionListener (SectorSelectionListener listener)
  // ---------------------------------------------------------------------------------//
  {
    diskLayoutImage.addSectorSelectionListener (listener);
  }

  // ---------------------------------------------------------------------------------//
  public void addSectorSelectionListener (SectorSelectionListener... listeners)
  // ---------------------------------------------------------------------------------//
  {
    for (SectorSelectionListener sectorSelectionListener : listeners)
      diskLayoutImage.addSectorSelectionListener (sectorSelectionListener);
  }

  // ---------------------------------------------------------------------------------//
  public void removeSectorSelectionListener (SectorSelectionListener listener)
  // ---------------------------------------------------------------------------------//
  {
    diskLayoutImage.removeSectorSelectionListener (listener);
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void diskSelected (DiskSelectedEvent event)
  // ---------------------------------------------------------------------------------//
  {
    setDisk (event.getFormattedDisk ());
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public void fileSelected (FileSelectedEvent event)
  // ---------------------------------------------------------------------------------//
  {
    // This can happen if a file is selected from a dual-dos disk
    checkCorrectDisk (event.appleFileSource.getFormattedDisk ());

    diskLayoutImage.setSelection (event.appleFileSource.getSectors ());
  }

  // ---------------------------------------------------------------------------------//
  class LayoutDetails
  // ---------------------------------------------------------------------------------//
  {
    Dimension block;
    Dimension grid;

    public LayoutDetails (FormattedDisk formattedDisk)
    {
      Disk disk = formattedDisk.getDisk ();
      block = new Dimension (disk.getBlockSize () == 256 ? SIZE : SIZE * 2, SIZE);
      grid = formattedDisk.getGridLayout ();
    }

    public Rectangle getLocation (DiskAddress da)
    {
      int y = da.getBlockNo () / grid.width;
      int x = da.getBlockNo () % grid.width;
      Rectangle r =
          new Rectangle (x * block.width, y * block.height, block.width, block.height);
      return r;
    }

    @Override
    public String toString ()
    {
      StringBuilder text = new StringBuilder ();
      text.append ("Block " + block + "\n");
      text.append ("Grid  " + grid);
      return text.toString ();
    }
  }

  // ---------------------------------------------------------------------------------//
  class Corner extends JComponent
  // ---------------------------------------------------------------------------------//
  {
    Color backgroundColor = Color.WHITE;
    boolean showHex = true;

    public Corner (boolean allowClick)
    {
      if (allowClick)
        addMouseListener (new MouseAdapter ()
        {
          @Override
          public void mouseClicked (MouseEvent e)
          {
            showHex = !showHex;
            verticalRuler.setHex (showHex);
            horizontalRuler.setHex (showHex);
          }
        });
    }

    @Override
    protected void paintComponent (Graphics g)
    {
      g.setColor (backgroundColor);
      g.fillRect (0, 0, getWidth (), getHeight ());
    }
  }

  @Override
  public void redo (RedoEvent event)
  {
    if (!event.type.equals ("SectorEvent"))
      return;

    // This can happen if sectors are selected from a dual-dos disk
    checkCorrectDisk (((SectorSelectedEvent) event.value).getFormattedDisk ());

    diskLayoutImage.redo (event);
  }

  private void checkCorrectDisk (FormattedDisk newDisk)
  {
    if (newDisk instanceof HybridDisk hybridDisk)
      newDisk = hybridDisk.getCurrentDisk ();           // never set to a hybrid disk
    if (newDisk != diskLayoutImage.getDisk ())
    {
      LayoutDetails layout = new LayoutDetails (newDisk);
      diskLayoutImage.setDisk (newDisk, layout);
      legendPanel.setDisk (newDisk, layout);
    }
  }
}