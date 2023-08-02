package com.bytezone.diskbrowser.duplicates;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import com.bytezone.diskbrowser.utilities.Utility;

// -----------------------------------------------------------------------------------//
public class DiskDetails
// -----------------------------------------------------------------------------------//
{
  private final File file;
  private long checksum;
  private final String rootName;            // full path without the root folder
  private final String shortName;           // file name in lower case
  private final String type;
  private final long size;

  private final List<DiskDetails> duplicateChecksums = new ArrayList<> ();
  private final List<DiskDetails> duplicateNames = new ArrayList<> ();

  private boolean isDuplicateName;
  private boolean isDuplicateChecksum;

  // ---------------------------------------------------------------------------------//
  public DiskDetails (File file, String rootName, String shortName, boolean doChecksum)
  // ---------------------------------------------------------------------------------//
  {
    this.file = file;
    this.rootName = rootName;
    this.shortName = shortName;
    this.type = Utility.getSuffix (shortName);
    this.size = file.length ();

    if (doChecksum)
      checksum = Utility.getChecksumValue (file);
    else
      checksum = 0;
  }

  // ---------------------------------------------------------------------------------//
  public File getFile ()
  // ---------------------------------------------------------------------------------//
  {
    return file;
  }

  // ---------------------------------------------------------------------------------//
  public void addDuplicateChecksum (DiskDetails diskDetails)
  // ---------------------------------------------------------------------------------//
  {
    if (this.checksum == diskDetails.checksum)
    {
      this.duplicateChecksums.add (diskDetails);
      diskDetails.isDuplicateChecksum = true;
    }
  }

  // ---------------------------------------------------------------------------------//
  public void addDuplicateName (DiskDetails diskDetails)
  // ---------------------------------------------------------------------------------//
  {
    if (this.shortName.equals (diskDetails.shortName))
    {
      this.duplicateNames.add (diskDetails);
      diskDetails.isDuplicateName = true;
    }
  }

  // ---------------------------------------------------------------------------------//
  public List<DiskDetails> getDuplicateChecksums ()
  // ---------------------------------------------------------------------------------//
  {
    return duplicateChecksums;
  }

  // ---------------------------------------------------------------------------------//
  public List<DiskDetails> getDuplicateNames ()
  // ---------------------------------------------------------------------------------//
  {
    return duplicateNames;
  }

  // ---------------------------------------------------------------------------------//
  public boolean isDuplicateChecksum ()
  // ---------------------------------------------------------------------------------//
  {
    return isDuplicateChecksum;
  }

  // ---------------------------------------------------------------------------------//
  public boolean isDuplicateName ()
  // ---------------------------------------------------------------------------------//
  {
    return isDuplicateName;
  }

  // ---------------------------------------------------------------------------------//
  public String getRootName ()
  // ---------------------------------------------------------------------------------//
  {
    return rootName;
  }

  // ---------------------------------------------------------------------------------//
  public String getType ()
  // ---------------------------------------------------------------------------------//
  {
    return type;
  }

  // ---------------------------------------------------------------------------------//
  public long getSize ()
  // ---------------------------------------------------------------------------------//
  {
    return size;
  }

  // ---------------------------------------------------------------------------------//
  public String getShortName ()
  // ---------------------------------------------------------------------------------//
  {
    return shortName;
  }

  // ---------------------------------------------------------------------------------//
  public String getFileName ()
  // ---------------------------------------------------------------------------------//
  {
    return file.getName ();
  }

  // ---------------------------------------------------------------------------------//
  public long calculateChecksum ()
  // ---------------------------------------------------------------------------------//
  {
    checksum = Utility.getChecksumValue (file);
    return checksum;
  }

  // ---------------------------------------------------------------------------------//
  public long getChecksum ()
  // ---------------------------------------------------------------------------------//
  {
    return checksum;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String toString ()
  // ---------------------------------------------------------------------------------//
  {
    return String.format ("%3d %1.1s %3d %1.1s %-40s  ", duplicateChecksums.size (),
        isDuplicateChecksum, duplicateNames.size (), isDuplicateName, rootName);
  }
}