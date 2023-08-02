package com.bytezone.diskbrowser.duplicates;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.RowFilter;
import javax.swing.ScrollPaneConstants;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumnModel;
import javax.swing.table.TableRowSorter;

import com.bytezone.diskbrowser.gui.DuplicateAction.DiskTableSelectionListener;
import com.bytezone.diskbrowser.utilities.NumberRenderer;
import com.bytezone.diskbrowser.utilities.Utility;

// -----------------------------------------------------------------------------------//
public class DisksWindow extends JFrame
// -----------------------------------------------------------------------------------//
{
  private final JTable table;

  private final JButton btnExport = new JButton ("Export");
  private final JButton btnDelete = new JButton ("Duplicates");
  private final JButton btnHide = new JButton ("Close");
  private final JButton btnTotals = new JButton ("Totals");
  private final JPanel topPanel = new JPanel ();

  private final List<JCheckBox> boxes = new ArrayList<> ();
  private TableRowSorter<DiskTableModel> sorter;
  private final CheckBoxActionListener checkBoxActionListener =
      new CheckBoxActionListener ();

  private DiskTableModel diskTableModel;
  private final RootFolderData rootFolderData;
  private final DeleteWindow deleteWindow;

  // ---------------------------------------------------------------------------------//
  public DisksWindow (RootFolderData rootFolderData)
  // ---------------------------------------------------------------------------------//
  {
    super (rootFolderData.getRootFolderPathText ());
    this.rootFolderData = rootFolderData;

    table = new JTable ();
    JScrollPane scrollPane =
        new JScrollPane (table, ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS,
            ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);

    table.setFillsViewportHeight (true);

    table.setShowGrid (true);
    table.setGridColor (Color.LIGHT_GRAY);

    add (scrollPane, BorderLayout.CENTER);

    JPanel panel = new JPanel ();
    panel.add (btnTotals);
    panel.add (btnHide);
    panel.add (btnExport);
    panel.add (btnDelete);
    add (panel, BorderLayout.SOUTH);

    topPanel.setLayout (new FlowLayout (FlowLayout.LEFT, 10, 5));
    add (topPanel, BorderLayout.NORTH);

    btnHide.setEnabled (true);
    btnExport.setEnabled (false);

    btnTotals.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        DisksWindow.this.rootFolderData.dialogTotals.setVisible (true);
      }
    });

    btnHide.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        setVisible (false);
      }
    });

    btnDelete.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        deleteWindow.setVisible (true);
      }
    });

    btnExport.addActionListener (new ActionListener ()
    {
      @Override
      public void actionPerformed (ActionEvent e)
      {
        CSVFileWriter.write (diskTableModel, table);
      }
    });

    scrollPane.setPreferredSize (new Dimension (1200, 693));
    setDefaultCloseOperation (HIDE_ON_CLOSE);

    deleteWindow = new DeleteWindow (rootFolderData);
  }

  // called from DuplicateSwingWorker
  // ---------------------------------------------------------------------------------//
  public void setTableData (final RootFolderData rootFolderData)
  // ---------------------------------------------------------------------------------//
  {
    diskTableModel = new DiskTableModel (rootFolderData);
    table.setModel (diskTableModel);

    int[] columnWidths = { 300, 300, 30, 50, 30, 100 };
    TableColumnModel tcm = table.getColumnModel ();
    for (int i = 0; i < columnWidths.length; i++)
      tcm.getColumn (i).setPreferredWidth (columnWidths[i]);

    // extra column if doing checksums
    if (rootFolderData.doChecksums)
      tcm.getColumn (6).setPreferredWidth (tcm.getColumn (4).getPreferredWidth ());

    tcm.getColumn (3).setCellRenderer (NumberRenderer.getIntegerRenderer ());

    sorter = new TableRowSorter<DiskTableModel> ((DiskTableModel) table.getModel ());
    table.setRowSorter (sorter);

    ListSelectionModel listSelectionModel = table.getSelectionModel ();
    listSelectionModel.addListSelectionListener (new ListSelectionListener ()
    {
      @Override
      public void valueChanged (ListSelectionEvent e)
      {
        if (e.getValueIsAdjusting ())
          return;

        ListSelectionModel lsm = (ListSelectionModel) e.getSource ();
        if (lsm.isSelectionEmpty ())
          return;

        table.scrollRectToVisible (
            new Rectangle (table.getCellRect (lsm.getMinSelectionIndex (), 0, true)));
        int selectedRow = table.getSelectedRow ();
        int actualRow = sorter.convertRowIndexToModel (selectedRow);

        DiskTableModel diskTableModel = (DiskTableModel) table.getModel ();
        DiskDetails diskDetails = diskTableModel.getDiskDetails (actualRow);

        long checksum = diskDetails.getChecksum ();
        if (checksum == 0)
          diskTableModel.updateChecksum (actualRow);

        for (DiskTableSelectionListener listener : rootFolderData.listeners)
          listener.diskSelected (diskDetails);
      }
    });

    table.addMouseListener (new MouseAdapter ()
    {
      @Override
      public void mousePressed (MouseEvent mouseEvent)
      {
        if (mouseEvent.getClickCount () == 2)
          deleteWindow.setVisible (true);
      }
    });

    for (int i = 0; i < Utility.getTotalSuffixes (); i++)
    {
      int total = rootFolderData.getTotalType (i);
      JCheckBox btn =
          new JCheckBox (String.format ("%s (%,d)", Utility.getSuffix (i), total));
      topPanel.add (btn);
      boxes.add (btn);

      if (total > 0)
      {
        btn.setSelected (true);
        btn.addActionListener (checkBoxActionListener);
      }
      else
        btn.setEnabled (false);
    }

    JTableHeader header = table.getTableHeader ();
    header.setFont (header.getFont ().deriveFont ((float) 13.0));

    pack ();
    setLocationRelativeTo (null);
    btnExport.setEnabled (true);

    setVisible (true);
  }

  // ---------------------------------------------------------------------------------//
  private String getFilterText ()
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder filterText = new StringBuilder ();

    for (JCheckBox box : boxes)
      if (box.isSelected ())
      {
        String text = box.getText ();
        int pos = text.indexOf (' ');
        filterText.append (text.substring (0, pos) + "|");
      }

    if (filterText.length () > 0)
      filterText.deleteCharAt (filterText.length () - 1);

    return filterText.toString ();
  }

  // ---------------------------------------------------------------------------------//
  class CheckBoxActionListener implements ActionListener
  // ---------------------------------------------------------------------------------//
  {
    @Override
    public void actionPerformed (ActionEvent e)
    {
      RowFilter<DiskTableModel, Object> rf = null;
      try
      {
        rf = RowFilter.regexFilter (getFilterText (), 2);
      }
      catch (java.util.regex.PatternSyntaxException exception)
      {
        return;
      }
      sorter.setRowFilter (rf);
    }
  }
}