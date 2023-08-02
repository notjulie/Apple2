package com.bytezone.diskbrowser.disk;

// -----------------------------------------------------------------------------------//
public class AppleDiskAddress implements DiskAddress
// -----------------------------------------------------------------------------------//
{
  private final int block;
  private final int track;
  private final int sector;
  public final Disk owner;

  private boolean zeroFlag;

  // ---------------------------------------------------------------------------------//
  public AppleDiskAddress (Disk owner, int block)
  // ---------------------------------------------------------------------------------//
  {
    this.owner = owner;
    this.block = block;
    int sectorsPerTrack = owner.getBlocksPerTrack ();
    if (sectorsPerTrack == 0)
    {
      track = 0;
      sector = 0;
    }
    else
    {
      track = block / sectorsPerTrack;
      sector = block % sectorsPerTrack;
    }
  }

  // ---------------------------------------------------------------------------------//
  public AppleDiskAddress (Disk owner, int track, int sector)
  // ---------------------------------------------------------------------------------//
  {
    this.owner = owner;
    zeroFlag = (track & 0x40) == 0x40;
    this.track = track & 0x3F;
    this.sector = sector & 0x1F;
    this.block = this.track * owner.getBlocksPerTrack () + this.sector;
  }

  // ---------------------------------------------------------------------------------//
  public boolean zeroFlag ()
  // ---------------------------------------------------------------------------------//
  {
    return zeroFlag;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public int compareTo (DiskAddress that)
  // ---------------------------------------------------------------------------------//
  {
    return this.block - that.getBlockNo ();
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public boolean matches (DiskAddress that)
  // ---------------------------------------------------------------------------------//
  {
    if (that == null)
      return false;
    return this.block == that.getBlockNo ();
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public boolean isZero ()
  // ---------------------------------------------------------------------------------//
  {
    return block == 0;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public int getBlockNo ()
  // ---------------------------------------------------------------------------------//
  {
    return block;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public int getSectorNo ()
  // ---------------------------------------------------------------------------------//
  {
    return sector;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public int getTrackNo ()
  // ---------------------------------------------------------------------------------//
  {
    return track;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public Disk getDisk ()
  // ---------------------------------------------------------------------------------//
  {
    return owner;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String toString ()
  // ---------------------------------------------------------------------------------//
  {
    return String.format ("[Block=%02X, Track=%02X, Sector=%02X, Zero=%s]", block, track,
        sector, zeroFlag);
  }
}