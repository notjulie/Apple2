package com.bytezone.diskbrowser.disk;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Enumeration;
import java.util.zip.GZIPInputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import com.bytezone.diskbrowser.cpm.CPMDisk;
import com.bytezone.diskbrowser.dos.DosDisk;
import com.bytezone.diskbrowser.infocom.InfocomDisk;
import com.bytezone.diskbrowser.nib.NibFile;
import com.bytezone.diskbrowser.nib.V2dFile;
import com.bytezone.diskbrowser.nib.WozFile;
import com.bytezone.diskbrowser.nufx.Binary2;
import com.bytezone.diskbrowser.nufx.NuFX;
import com.bytezone.diskbrowser.pascal.PascalDisk;
import com.bytezone.diskbrowser.prodos.ProdosDisk;
import com.bytezone.diskbrowser.utilities.FileFormatException;
import com.bytezone.diskbrowser.utilities.HexFormatter;
import com.bytezone.diskbrowser.utilities.Utility;
import com.bytezone.diskbrowser.wizardry.Wizardry4BootDisk;
import com.bytezone.diskbrowser.wizardry.WizardryScenarioDisk;

// -----------------------------------------------------------------------------------//
public class DiskFactory
// -----------------------------------------------------------------------------------//
{
  private static boolean debug = false;

  private static final int DISK_800K = 819200;
  private static final int DISK_143K = 143360;
  private static final int DISK_116K = 116480;

  private static NuFX nuFX;
  private static Binary2 binary2;

  // ---------------------------------------------------------------------------------//
  private DiskFactory ()
  // ---------------------------------------------------------------------------------//
  {
  }

  // ---------------------------------------------------------------------------------//
  public static FormattedDisk createDisk (File file)
  // ---------------------------------------------------------------------------------//
  {
    return createDisk (file.getAbsolutePath ());
  }

  // ---------------------------------------------------------------------------------//
  public static FormattedDisk createDisk (String pathName)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("\nFactory : " + pathName);

    nuFX = null;
    binary2 = null;

    File file = new File (pathName);
    if (!file.exists ())
      return null;

    String suffix = pathName.substring (pathName.lastIndexOf (".") + 1).toLowerCase ();
    Boolean compressed = false;
    Path originalPath = Paths.get (pathName);

    if ("gz".equals (suffix))
    {
      if (debug)
        System.out.println (" ** gzip **");
      try
      {
        InputStream in = new GZIPInputStream (new FileInputStream (pathName));
        File tmp = File.createTempFile ("gzip", null);
        FileOutputStream fos = new FileOutputStream (tmp);

        int bytesRead;
        byte[] buffer = new byte[1024];
        while ((bytesRead = in.read (buffer)) > 0)
          fos.write (buffer, 0, bytesRead);

        fos.close ();
        in.close ();
        tmp.deleteOnExit ();

        suffix = Utility.getSuffix (file.getName ());     // ignores the .gz and .zip
        file = tmp;
        compressed = true;
      }
      catch (IOException e)  // can get EOFException: Unexpected end of ZLIB input stream
      {
        System.out.printf ("Error unpacking: %s%n", file.getAbsolutePath ());
        return null;
      }
    }
    else if ("zip".equals (suffix))
    {
      if (debug)
        System.out.println (" ** zip **");
      try
      {
        ZipFile zipFile = new ZipFile (pathName);
        Enumeration<? extends ZipEntry> entries = zipFile.entries ();

        while (entries.hasMoreElements ())        // loop until first valid name
        {
          ZipEntry entry = entries.nextElement ();
          if (Utility.validFileType (entry.getName ()))
          {
            InputStream stream = zipFile.getInputStream (entry);
            File tmp = File.createTempFile ("zip", null);
            FileOutputStream fos = new FileOutputStream (tmp);

            int bytesRead;
            byte[] buffer = new byte[1024];
            while ((bytesRead = stream.read (buffer)) > 0)
              fos.write (buffer, 0, bytesRead);

            stream.close ();
            fos.close ();
            tmp.deleteOnExit ();

            suffix = Utility.getSuffix (file.getName ());   // ignores the .gz and .zip
            file = tmp;
            compressed = true;

            break;
          }
        }

        zipFile.close ();
      }
      catch (IOException e)
      {
        System.out.printf ("Error unpacking: %s%n", file.getAbsolutePath ());
        return null;
      }
    }

    if ("sdk".equals (suffix)         // NuFX disk
        || "shk".equals (suffix)      // NuFX files or disk
        || "bxy".equals (suffix))     // NuFX in Bin2
    {
      if (debug)
        System.out.println (" ** sdk/shk/bxy **");
      try
      {
        nuFX = new NuFX (file.toPath ());
        if (nuFX.getTotalDisks () == 0 && nuFX.getTotalFiles () == 0)
        {
          if (debug)
            System.out.println ("Empty NuFX file");
          return null;
        }

        byte[] diskBuffer = nuFX.getDiskBuffer ();
        if (diskBuffer == null)
          return null;

        File tmp = File.createTempFile (suffix, null);
        FileOutputStream fos = new FileOutputStream (tmp);
        fos.write (diskBuffer);
        fos.close ();
        tmp.deleteOnExit ();
        file = tmp;
        suffix = "dsk";
        compressed = true;
      }
      catch (Exception e)
      {
        //        e.printStackTrace ();
        if (e.getMessage () == null)
          System.out.println (e);
        else
          System.out.println (e.getMessage ());
        System.out.printf ("Error unpacking: %s%n", file.getAbsolutePath ());
        // System.out.println (nuFX);
        return null;
      }
    }
    else if ("bny".equals (suffix) || "bqy".equals (suffix))     // Binary2 uncompressed files
    {
      if (debug)
        System.out.println (" ** bny/bqy **");
      try
      {
        binary2 = new Binary2 (file.toPath ());
        byte[] diskBuffer = binary2.getDiskBuffer ();

        File tmp = File.createTempFile (suffix, null);
        FileOutputStream fos = new FileOutputStream (tmp);
        fos.write (diskBuffer);
        fos.close ();

        tmp.deleteOnExit ();
        file = tmp;
        suffix = "dsk";
        compressed = true;
      }
      catch (Exception e)
      {
        // e.printStackTrace ();
        System.out.println (e.getMessage ());
        System.out.printf ("Error unpacking: %s%n", file.getAbsolutePath ());
        // System.out.println (binary2);
        return null;
      }
    }
    else if ("bsq".equals (suffix))
    {
      if (debug)
        System.out.println (" ** bsq **");
      byte[] prefix = getPrefix (file);
      System.out.println (HexFormatter.format (prefix));
      String key = "FiLeStArTfIlEsTaRt";
    }

    FormattedDisk disk = null;
    FormattedDisk disk2 = null;

    if (suffix.equals ("hdv"))
    {
      if (debug)
        System.out.println (" ** hdv **");
      FormattedDisk prodosDisk = checkHardDisk (file);
      if (prodosDisk != null)
        return prodosDisk;

      disk = check2mgDisk (file);
      if (disk != null)
      {
        if (compressed)
          disk.setOriginalPath (originalPath);
        return disk;
      }

      AppleDisk appleDisk = new AppleDisk (file, (int) file.length () / 4096, 8);
      return new DataDisk (appleDisk);
    }

    if (suffix.equals ("2mg"))
    {
      if (debug)
        System.out.println (" ** 2mg **");
      disk = check2mgDisk (file);
      if (disk != null)
      {
        if (compressed)
          disk.setOriginalPath (originalPath);
        return disk;
      }

      AppleDisk appleDisk = new AppleDisk (file, (int) file.length () / 4096, 8);
      return new DataDisk (appleDisk);
    }

    if (suffix.equals ("img") || suffix.equals ("dimg"))
    {
      disk = checkDiskCopyDisk (file);
      if (disk != null)
        return disk;
    }

    // Toolkit.do = 143488
    if (((suffix.equals ("po") || suffix.equals ("dsk") || suffix.equals ("do"))
        && file.length () > DISK_143K))
    {
      if (file.length () < 143500)        // slightly bigger than a floppy
      {
        System.out.println ("File length is wrong: " + file.length ());
        disk = checkDos (new AppleDisk (file, 35, 16));
        if (disk != null)
          return check (disk);
      }

      if (debug)
        System.out.printf ("Checking po or dsk hard drive: %,d%n", file.length ());

      disk = checkHardDisk (file);
      if (disk != null)
      {
        if (compressed)
          disk.setOriginalPath (originalPath);
        return check (disk);
      }

      if (file.length () == DISK_800K)         // 800K 3.5"
      {
        if (debug)
          System.out.println ("UniDos ?");
        // 2 x 400k disk images
        AppleDisk appleDisk1 = new AppleDisk (file, 50, 32);
        AppleDisk appleDisk2 = new AppleDisk (file, 50, 32, 0x64000);
        disk = checkUnidos (appleDisk1, 1);
        disk2 = checkUnidos (appleDisk2, 2);
        if (disk != null && disk2 != null)
          return new HybridDisk (disk, disk2);
      }

      if (debug)
        System.out.println ("  Creating a data disk from bad length");

      try
      {
        AppleDisk appleDisk = new AppleDisk (file, (int) file.length () / 4096, 8);
        if (debug)
          System.out.println ("  created data disk");
        return new DataDisk (appleDisk);
      }
      catch (FileFormatException e)
      {
        if (debug)
          System.out.println ("  Creating AppleDisk failed");
        return null;
      }
    }

    if (suffix.equals ("woz"))
    {
      if (debug)
        System.out.println ("Checking woz");
      try
      {
        WozFile wozFile = new WozFile (file);

        if (wozFile.getSectorsPerTrack () == 13)
        {
          AppleDisk appleDisk = new AppleDisk (wozFile, 35, 13);
          disk = checkDos (appleDisk);
          return disk == null ? new DataDisk (appleDisk) : disk;
        }

        if (wozFile.getSectorsPerTrack () == 16)
        {
          if (wozFile.getDiskType () == 2)
          {
            if (debug)
              System.out.println ("Checking woz 3.5");
            AppleDisk disk800 = new AppleDisk (wozFile, 100 * wozFile.getSides (), 8);
            if (ProdosDisk.isCorrectFormat (disk800))
            {
              if (debug)
                System.out.println ("  --> PRODOS hard disk");
              disk = new ProdosDisk (disk800);
            }
            else
              disk = new DataDisk (disk800);
          }
          else
          {
            AppleDisk appleDisk256 = new AppleDisk (wozFile, wozFile.getTracks (), 16);
            disk = checkDos (appleDisk256);
            if (disk == null)
              disk = checkProdos (new AppleDisk (wozFile, 35, 8));
            if (disk == null)
              disk = checkPascalDisk (new AppleDisk (wozFile, 35, 8));
            if (disk == null)
              disk = new DataDisk (appleDisk256);
          }
        }

        disk.setOriginalPath (originalPath);          // allow Save converted disk...

        return disk;
      }
      catch (Exception e)
      {
        System.out.println (e);
        return null;
      }
    }

    if (suffix.equals ("v2d"))
    {
      V2dFile v2dDisk = new V2dFile (file);
      AppleDisk appleDisk256 = new AppleDisk (v2dDisk, 35, 16);
      disk = checkDos (appleDisk256);
      if (disk == null)
        disk = checkProdos (new AppleDisk (v2dDisk, 35, 8));
      if (disk == null)
        disk = new DataDisk (appleDisk256);
      return disk;
    }

    if (suffix.equals ("nib"))          // not implemented yet
    {
      if (debug)
        System.out.println (" ** nib **");

      NibFile nibDisk = new NibFile (file);
      AppleDisk appleDisk16 = new AppleDisk (nibDisk);
      disk = checkDos (appleDisk16);
      return null;
    }

    long length = file.length ();

    if (length == DISK_116K)           // 13 sector floppy disk
    {
      if (debug)
        System.out.println (" ** 13 sector **");
      if (!suffix.equals ("d13"))
        System.out.printf ("%s should have a d13 suffix%n", file.getName ());

      AppleDisk appleDisk = new AppleDisk (file, 35, 13);
      disk = checkDos (appleDisk);
      return disk == null ? new DataDisk (appleDisk) : disk;
    }

    if (length != DISK_143K)          // 16 sector floppy disk
    {
      System.out.printf ("%s: invalid file length : %,d%n", file.getName (), file.length ());
      return null;
    }

    AppleDisk appleDisk256 = new AppleDisk (file, 35, 16);
    AppleDisk appleDisk512 = new AppleDisk (file, 35, 8);

    if (true)
    {
      // long checksum = appleDisk256.getBootChecksum ();
      long checksum = 0;

      if (checksum == 227968344L)       // empty boot sector
      {
        // could be wizardry data, visialc data ...
        if (debug)
          System.out.println ("  empty sector checksum : " + checksum);
      }
      else if (checksum == 3176296590L  // HIGHRTXT.DSK
          || checksum == 108825457L     // ARCBOOT.DSK (looping)
          || checksum == 1439356606L    //
          || checksum == 1550012074L    //
          || checksum == 1614602459L    //
          || checksum == 940889336L     //
          || checksum == 2936955085L    //
          || checksum == 1348415927L    //
          || checksum == 3340889101L    //
          || checksum == 18315788L      //
          || checksum == 993895235L     //
          || checksum == 2378342794L)   // LazerPascal1.dsk
      {
        if (debug)
          System.out.println ("  known DOS checksum : " + checksum);
        disk = checkDos (appleDisk256);
        // disk2 = checkProdos (appleDisk512); // no need for this
        if (disk2 != null && disk != null)      // should be impossible
        {
          if (debug)
            System.out.println ("  --> Dual dos/prodos 1");
          System.out.println ("** impossible **");
          disk = new HybridDisk (disk, disk2);
        }
      }
      else if (checksum == 1737448647L      //
          || checksum == 170399908L         //
          || checksum == 990032697)         // Apple Assembly Line
      {
        if (debug)
          System.out.println ("  known PRODOS checksum : " + checksum);
        disk = checkProdos (appleDisk512);
        disk2 = checkDos (appleDisk256);
        if (disk2 != null && disk != null)
        {
          if (debug)
            System.out.println ("  --> Dual prodos/dos 2");
          disk = new HybridDisk (disk, disk2);
        }
      }
      else if (checksum == 2803644711L    // Apple Pascal disk 0
          || checksum == 3317783349L      //
          || checksum == 1728863694L      // Wizardry_I_boot.dsk
          || checksum == 198094178L)      //
      {
        if (debug)
          System.out.println ("  known PASCAL checksum : " + checksum);
        disk = checkPascalDisk (appleDisk512);
        disk2 = checkDos (appleDisk256);
        if (disk2 != null)
          disk = new HybridDisk (disk, disk2);
      }
      else if (checksum == 3028642627L    //
          || checksum == 2070151659L)     // Enchanter
      {
        if (debug)
          System.out.println ("  known INFOCOM checksum : " + checksum);
        disk = checkInfocomDisk (appleDisk256);
      }
      else if (debug)
        System.out.println ("  unknown checksum : " + checksum);

      // else if (checksum == 1212926910L || checksum == 1365043894L
      // || checksum == 2128073918L)
      // disk = checkCPMDisk (file);

      // System.out.println (checksum);

      if (disk != null)
      {
        if (compressed)
          disk.setOriginalPath (originalPath);
        return check (disk);
      }

      // empty boot sector
      if (checksum != 227968344L && false)
        System.out.println ("Unknown checksum : " + checksum + " : " + pathName);
    }

    if (debug)
      System.out.println ("  checksum no help");
    if (debug)
      System.out.println ("  Suffix : " + suffix);

    if (suffix.equals ("dsk") || suffix.equals ("do"))
    {
      disk = checkDos (appleDisk256);
      if (disk == null)
        disk = checkProdos (appleDisk512);
      else
      {
        if (debug)
          System.out.println ("Checking DualDos disk");

        disk2 = checkProdos (appleDisk512);
        if (disk2 != null)
        {
          disk = new HybridDisk (disk, disk2);
          return disk;
        }

        AppleDisk appleDisk = new AppleDisk (file, 35, 16);
        disk2 = checkCPMDisk (appleDisk);
        if (disk2 != null)
        {
          disk = new HybridDisk (disk, disk2);
          return disk;
        }

        disk2 = checkPascalDisk (appleDisk512);
        if (disk2 != null)
        {
          disk = new HybridDisk (disk, disk2);
          return disk;
        }
      }
    }
    else if (suffix.equals ("po"))
    {
      disk = checkProdos (appleDisk512);
      if (disk == null)
        disk = checkDos (appleDisk256);
    }

    if (disk == null)
      disk = checkPascalDisk (appleDisk512);

    if (disk == null)
      disk = checkCPMDisk (appleDisk256);

    if (disk == null)
    {
      disk2 = checkInfocomDisk (appleDisk256);
      if (disk2 != null)
        disk = disk2;
    }

    if (disk == null)
      disk = new DataDisk (appleDisk256);

    if (debug)
      System.out
          .println ("Factory creating disk : " + disk.getDisk ().getFile ().getAbsolutePath ());

    if (disk != null && compressed)
      disk.setOriginalPath (originalPath);

    return check (disk);
  }

  // ---------------------------------------------------------------------------------//
  private static FormattedDisk check (FormattedDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (disk.getDisk () instanceof AppleDisk appleDisk)
    {
      if (nuFX != null)
        appleDisk.setNuFX (nuFX);
      else if (binary2 != null)
        appleDisk.setBinary2 (binary2);
    }

    return disk;
  }

  // ---------------------------------------------------------------------------------//
  private static DosDisk checkDos (AppleDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking DOS disk");

    try
    {
      if (DosDisk.isCorrectFormat (disk))
      {
        if (debug)
          System.out.println ("  --> DOS");
        DosDisk dosDisk = new DosDisk (disk);
        // disk.setDosVersion (dosDisk.getVersion ());

        return dosDisk;
      }
    }
    catch (Exception e)
    {
      e.printStackTrace ();
    }
    if (debug)
      System.out.println ("  not a DOS disk");
    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static ProdosDisk checkProdos (AppleDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking Prodos disk");

    try
    {
      if (ProdosDisk.isCorrectFormat (disk))
      {
        if (debug)
          System.out.println ("  --> PRODOS");
        return new ProdosDisk (disk);
      }
    }
    catch (Exception e)
    {
    }
    if (debug)
      System.out.println ("  not a Prodos disk");
    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static DosDisk checkUnidos (AppleDisk disk, int side)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking UniDOS disk");

    try
    {
      if (DosDisk.isCorrectFormat (disk))
      {
        if (debug)
          System.out.println ("  --> UniDOS");
        return new DosDisk (disk, side);
      }
    }
    catch (Exception e)
    {
      e.printStackTrace ();
    }
    if (debug)
      System.out.println ("  not a UniDOS disk");
    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static FormattedDisk checkHardDisk (File file)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
    {
      System.out.println ("\nChecking Prodos/Pascal hard disk");
      System.out.printf ("Total blocks : %f%n", (float) file.length () / 512);
      System.out.printf ("Total tracks : %f%n", (float) file.length () / 4096);
      System.out.printf ("File length  : %d%n", file.length ());
      System.out.println ();
    }

    // assumes a sector is 512 bytes
    if ((file.length () % 512) != 0)
    {
      if (debug)
        System.out.printf ("file length not divisible by 512 : %,d%n%n", file.length ());
      return null;
    }

    try
    {
      // extend the file if necessary
      int tracks = (int) (file.length () - 1) / 4096 + 1;
      if (tracks * 4096 != file.length ())
      {
        System.out.println ("*** extended ***");     // System Addons.hdv
      }
      AppleDisk disk;
      // if (nuFX == null)
      disk = new AppleDisk (file, tracks, 8);
      // else
      // disk = new AppleDisk (file, tracks, 8, nuFX);

      if (ProdosDisk.isCorrectFormat (disk))
      {
        if (debug)
          System.out.println ("  --> PRODOS hard disk");
        return new ProdosDisk (disk);
      }

      if (PascalDisk.isCorrectFormat (disk, debug))
      {
        if (debug)
          System.out.println ("  --> Pascal hard disk");
        return new PascalDisk (disk);
      }
    }
    catch (Exception e)
    {
      System.out.println (e);
      e.printStackTrace ();
      System.out.println ("Prodos hard disk had error");
    }

    if (debug)
      System.out.println ("  not a Prodos/Pascal hard disk\n");

    return null;
  }

  /*
  offset | size | description
  ------ | ---- | -----------
  +$000  | Long |  The integer constant '2IMG'. This integer should be little-endian,
                   so on the Apple IIgs, this is equivalent to the four characters
                   'GMI2'; in ORCA/C 2.1, you can use the integer constant '2IMG'.
  +$004  | Long |  A four-character tag identifying the application that created the
                   file.
  +$008  | Word |  The length of this header, in bytes. Should be 52.
  +$00A  | Word |  The version number of the image file format. Should be 1.
  +$00C  | Long |  The image format. See table below.
  +$010  | Long |  Flags. See table below.
  +$014  | Long |  The number of 512-byte blocks in the disk image. This value should
                   be zero unless the image format is 1 (ProDOS order).
  +$018  | Long |  Offset to the first byte of the first block of the disk in the image
                   file, from the beginning of the file. The disk data must come before
                   the comment and creator-specific chunks.
  +$01C  | Long |  Length of the disk data in bytes. This should be the number of
                   blocks * 512.
  +$020  | Long |  Offset to the first byte of the image comment. Can be zero if
                   there's no comment. The comment must come after the data chunk, but
                   before the creator-specific chunk. The comment, if it exists, should
                   be raw text; no length byte or C-style null terminator byte is
                   required (that's what the next field is for).
  +$024  | Long |  Length of the comment chunk. Zero if there's no comment.
  +$028  | Long |  Offset to the first byte of the creator-specific data chunk, or zero
                   if there is none.
  +$02C  | Long |  Length of the creator-specific chunk; zero if there is no
                   creator-specific data.
  +$030  | 16 bytes |  Reserved space; this pads the header to 64 bytes. These values
                   must all be zero.
  */

  // ---------------------------------------------------------------------------------//
  private static FormattedDisk check2mgDisk (File file)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking 2mg disk");

    try
    {
      AppleDisk disk = new AppleDisk (file, 0, 0);
      if (disk.getTotalBlocks () > 0)
      {
        if (ProdosDisk.isCorrectFormat (disk))
          return new ProdosDisk (disk);

        if (file.length () == 143424)
        {
          disk.switchToDos ();                    // switch sector size
          if (DosDisk.isCorrectFormat (disk))
            return new DosDisk (disk);
        }
      }
    }
    catch (Exception e)
    {
      e.printStackTrace ();
    }
    if (debug)
      System.out.println ("Not a 2mg disk");

    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static FormattedDisk checkDiskCopyDisk (File file)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking DiskCopy disk");

    try
    {
      AppleDisk disk = new AppleDisk (file, 0, 0);
      if (disk.getTotalBlocks () > 0)
      {
        if (ProdosDisk.isCorrectFormat (disk))
          return new ProdosDisk (disk);
      }
    }
    catch (Exception e)
    {
      e.printStackTrace ();
    }

    if (debug)
      System.out.println ("Not a DiskCopy disk");

    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static FormattedDisk checkPascalDisk (AppleDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking Pascal disk");

    File file = disk.getFile ();

    if (!PascalDisk.isCorrectFormat (disk, debug))
      return null;

    if (debug)
      System.out.println ("Pascal disk OK - Checking Wizardry disk");

    if (WizardryScenarioDisk.isWizardryFormat (disk, debug))
      return new WizardryScenarioDisk (disk);

    if (debug)
      System.out.println ("Not a Wizardry 1-3 disk");

    // check for compressed disk
    if (file.getName ().endsWith (".tmp"))
      return new PascalDisk (disk);       // complicated joining up compressed disks

    if (Wizardry4BootDisk.isWizardryIVorV (disk, debug))
    {
      if (debug)
        System.out.println ("checking Wizardry IV or V");

      String fileName = file.getAbsolutePath ().toLowerCase ();
      int pos = fileName.lastIndexOf ('.');
      char c = fileName.charAt (pos - 1);                     // '1' (wiz4) or 'a' (wiz5)
      int requiredDisks = c == '1' ? 7 : c == 'a' ? 10 : 0;

      if (debug)
        System.out.printf ("Required disks: %d%n", requiredDisks);

      if (requiredDisks > 0)
      {
        // collect extra data disks
        AppleDisk[] disks = new AppleDisk[requiredDisks];

        disks[0] = new AppleDisk (file, 256, 8);           // will become a PascalDisk
        disks[0].setInterleave (1);

        disks[1] = new AppleDisk (file, 256, 8);           // will remain a DataDisk
        disks[1].setInterleave (1);

        if (pos > 0 && requiredDisks > 0)
        {
          if (collectDataDisks (file.getAbsolutePath (), pos, disks))
            return new Wizardry4BootDisk (disks);
        }
      }
    }
    if (debug)
      System.out.println ("Not a Wizardry IV or V disk");

    PascalDisk pascalDisk = new PascalDisk (disk);
    return pascalDisk;
  }

  // ---------------------------------------------------------------------------------//
  private static boolean collectDataDisks (String fileName, int dotPos, AppleDisk[] disks)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Collecting Wizardry disks");

    char c = fileName.charAt (dotPos - 1);
    String suffix = fileName.substring (dotPos + 1);

    for (int i = 2; i < disks.length; i++)
    {
      String old = new String (c + "." + suffix);
      String rep = new String ((char) (c + i - 1) + "." + suffix);
      File f = new File (fileName.replace (old, rep));

      if (debug)
        System.out.println (f);

      if (!f.exists () || !f.isFile ())
        return false;

      disks[i] = new AppleDisk (f, 35, 8);
      disks[i].setInterleave (1);
    }

    return true;
  }

  // ---------------------------------------------------------------------------------//
  private static InfocomDisk checkInfocomDisk (AppleDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking Infocom disk");

    if (InfocomDisk.isCorrectFormat (disk))
    {
      if (debug)
        System.out.println ("  --> INFOCOM");
      return new InfocomDisk (disk);
    }

    if (debug)
      System.out.println ("Not an InfocomDisk disk");

    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static CPMDisk checkCPMDisk (AppleDisk disk)
  // ---------------------------------------------------------------------------------//
  {
    if (debug)
      System.out.println ("Checking CPM disk");

    if (CPMDisk.isCorrectFormat (disk))
      return new CPMDisk (disk);

    if (debug)
      System.out.println ("Not a CPM disk");

    return null;
  }

  // ---------------------------------------------------------------------------------//
  private static byte[] getPrefix (File file)
  // ---------------------------------------------------------------------------------//
  {
    byte[] buffer = new byte[1024];
    try (BufferedInputStream bis = new BufferedInputStream (new FileInputStream (file)))
    {
      bis.read (buffer);
    }
    catch (IOException e)
    {
      e.printStackTrace ();
      System.exit (1);
    }

    return buffer;
  }
}