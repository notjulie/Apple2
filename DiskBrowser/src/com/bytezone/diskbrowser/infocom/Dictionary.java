package com.bytezone.diskbrowser.infocom;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import com.bytezone.diskbrowser.applefile.AbstractFile;
import com.bytezone.diskbrowser.utilities.HexFormatter;

// -----------------------------------------------------------------------------------//
class Dictionary extends AbstractFile
// -----------------------------------------------------------------------------------//
{
  private final Map<Integer, ZString> dictionary;
  private final int totalEntries;
  private final int totalSeparators;
  private final int dictionaryPtr, dictionarySize;
  private final int entryLength;
  private final String separators;

  Map<Integer, List<WordEntry>> synonymList = new TreeMap<> ();

  // ---------------------------------------------------------------------------------//
  Dictionary (Header header)
  // ---------------------------------------------------------------------------------//
  {
    super ("Dictionary", header.buffer);

    dictionaryPtr = header.dictionaryOffset;
    dictionary = new TreeMap<> ();

    totalSeparators = buffer[dictionaryPtr] & 0xFF;

    StringBuilder sep = new StringBuilder ();
    for (int i = 0; i < totalSeparators; i++)
      sep.append ((char) (buffer[dictionaryPtr + i + 1] & 0xFF));
    separators = sep.toString ();

    int ptr = dictionaryPtr + totalSeparators + 1;
    entryLength = buffer[ptr++] & 0xFF;

    totalEntries = header.getWord (ptr);

    ptr += 2;
    int count = 0;
    for (int i = 0; i < totalEntries; i++)
    {
      ZString string = new ZString (header, ptr);
      dictionary.put (ptr, string);
      WordEntry wordEntry = new WordEntry (string, count++);

      // add the WordEntry to the appropriate list
      List<WordEntry> wordEntryList = synonymList.get (wordEntry.key);
      if (wordEntryList == null)
      {
        wordEntryList = new ArrayList<> ();
        synonymList.put (wordEntry.key, wordEntryList);
      }
      wordEntryList.add (wordEntry);

      // check for words with the direction flag
      if ((buffer[ptr + 4] & 0x10) != 0)
      {
        int b1 = buffer[ptr + 5] & 0xFF;
        int b2 = buffer[ptr + 6] & 0xFF;
        int property = b2 == 0 ? b1 : b2;
        property &= 0x1F;
        String propertyName = header.getPropertyName (property);

        System.out.printf ("%02X  %s%n", property, string.value);
        //        if (property >= header.propertyNames.length)
        //          System.out.println ("Illegal property");
        //        else
        if (propertyName == null || propertyName.length () > string.value.length ())
          header.propertyNames[property] = string.value;
      }
      ptr += entryLength;
    }

    dictionarySize = totalSeparators + 3 + entryLength * totalEntries;

    for (int i = 1; i < header.propertyNames.length; i++)
      if (header.propertyNames[i] == null)
        header.propertyNames[i] = i + "";

    // testing (only works in Zork 1)
    if (false)
    {
      if (header.propertyNames[4].equals ("4"))
        header.propertyNames[4] = "PSEUDO";
      if (header.propertyNames[5].equals ("5"))
        header.propertyNames[5] = "GLOBAL";
      if (header.propertyNames[6].equals ("6"))
        header.propertyNames[6] = "VTYPE";
      if (header.propertyNames[7].equals ("7"))
        header.propertyNames[7] = "STRENGTH";
      if (header.propertyNames[10].equals ("10"))
        header.propertyNames[10] = "CAPACITY";
      if (header.propertyNames[12].equals ("12"))
        header.propertyNames[12] = "TVALU";
      if (header.propertyNames[13].equals ("13"))
        header.propertyNames[13] = "VALUE";
      if (header.propertyNames[15].equals ("15"))
        header.propertyNames[15] = "SIZE";
      if (header.propertyNames[16].equals ("16"))
        header.propertyNames[16] = "ADJ";
    }

    // 04    = PSEUDO    (property 4)
    // 05    = GLOBAL    (property 5)
    // 06    = VTYPE     (property 6)
    // 07    = STRENGTH  (property 7)
    // STR3  = TEXT      (property 8)
    // CODE2 = DESCFCN   (property 9)
    // 0A    = CAPACITY  (property 10)
    // STR1  = LDESC     (property 11)
    // 0C    = TVALUE    (property 12)   value in trophy case
    // 0D    = VALUE     (property 13)
    // STR2  = FDESC     (property 14)
    // 0F    = SIZE      (property 15)
    // 10    = ADJ       (property 16)
    // CODE1 = ACTION    (property 17)
    // 12    = DICT      (property 18)

    // 13  land
    // 14  out
    // 15  in, inside, into
    // 16  d, down
    // 17  u, up
    // 18  sw, southw
    // 19  se, southe
    // 1A  nw, northw
    // 1B  ne, northe
    // 1C  s, south
    // 1D  w, west
    // 1E  e, east
    // 1F  n, north
  }

  // ---------------------------------------------------------------------------------//
  public boolean containsWordAt (int address)
  // ---------------------------------------------------------------------------------//
  {
    return dictionary.containsKey (address);
  }

  // ---------------------------------------------------------------------------------//
  public String wordAt (int address)
  // ---------------------------------------------------------------------------------//
  {
    if (dictionary.containsKey (address))
      return dictionary.get (address).value;
    return "dictionary can't find word @ " + address;
  }

  // ---------------------------------------------------------------------------------//
  public List<String> getVerbs (int value)
  // ---------------------------------------------------------------------------------//
  {
    List<String> words = new ArrayList<> ();
    int ptr = dictionaryPtr + totalSeparators + 4;

    for (ZString word : dictionary.values ())
    {
      int b1 = buffer[ptr + 4] & 0xFF;
      int b2 = buffer[ptr + 5] & 0xFF;
      int b3 = buffer[ptr + 6] & 0xFF;
      // mask seems to be 0x40
      if ((b1 == 0x41 && b2 == value)
          || ((b1 == 0x62 || b1 == 0xC0 || b1 == 0x44) && b3 == value))
        words.add (word.value);
      ptr += entryLength;
    }
    return words;
  }

  // ---------------------------------------------------------------------------------//
  public List<String> getPrepositions (int value)
  // ---------------------------------------------------------------------------------//
  {
    List<String> words = new ArrayList<> ();
    int ptr = dictionaryPtr + totalSeparators + 4;

    for (ZString word : dictionary.values ())
    {
      int b1 = buffer[ptr + 4] & 0xFF;
      int b2 = buffer[ptr + 5] & 0xFF;
      int b3 = buffer[ptr + 6] & 0xFF;
      // mask seems to be 0x08
      if (((b1 == 0x08 || b1 == 0x18 || b1 == 0x48) && b2 == value)
          || ((b1 == 0x1B || b1 == 0x0C || b1 == 0x2A) && b3 == value))
        words.add (word.value);
      ptr += entryLength;
    }
    return words;
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String getHexDump ()
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder text = new StringBuilder ();
    text.append (HexFormatter.format (buffer, dictionaryPtr, dictionarySize));
    return text.toString ();
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String getText ()
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder text = new StringBuilder ();

    text.append (String.format ("Entries    : %,6d%n", totalEntries));
    text.append (String.format ("Separators : %s%n", separators));
    text.append (
        String.format ("Offset     : %,6d   %04X%n%n", dictionaryPtr, dictionaryPtr));

    int count = 0;
    int ptr = dictionaryPtr + totalSeparators + 4;

    for (ZString word : dictionary.values ())
    {
      String bits = Integer.toBinaryString (buffer[ptr + 4] & 0xFF);
      if (bits.length () < 8)
        bits = "00000000".substring (bits.length ()) + bits;

      text.append (String.format ("%04X   %3d   %-6s   %s  %s", ptr, count++, word.value,
          bits, HexFormatter.getHexString (buffer, ptr + 4, entryLength - 4)));
      int b1 = buffer[ptr + 4] & 0xFF;
      int b2 = buffer[ptr + 5] & 0xFF;
      int b3 = buffer[ptr + 6] & 0xFF;
      if (b1 == 65)
        text.append (String.format ("  %3d%n", b2));
      else if (b1 == 98 || b1 == 0xC0 || b1 == 0x44)
        text.append (String.format ("  %3d%n", b3));
      else
        text.append ("\n");
      ptr += entryLength;
    }

    if (false)
    {
      int lastValue = 0;
      for (List<WordEntry> list : synonymList.values ())
      {
        WordEntry wordEntry = list.get (0);
        if (wordEntry.flags != lastValue)
        {
          lastValue = wordEntry.flags;
          text.append ("\n");
        }

        if (wordEntry.flags == 0x80)            // nouns are all in one entry
        {
          for (WordEntry we : list)
            text.append (we + "\n");
          text.deleteCharAt (text.length () - 1);
        }
        else
          text.append (wordEntry);

        if ((buffer[wordEntry.word.startPtr + 4] & 0x10) != 0)
          text.append ("  direction");

        text.append ("\n");
      }
    }

    if (true)
    {
      text.append ("\n");
      int lastValue = 0;

      for (int bit = 1; bit < 256; bit *= 2)
      {
        String bits = Integer.toBinaryString (bit & 0xFF);
        if (bits.length () < 8)
          bits = "00000000".substring (bits.length ()) + bits;

        text.append (String.format ("Bits:                 %s%n", bits));
        for (List<WordEntry> list : synonymList.values ())
        {
          WordEntry wordEntry = list.get (0);
          if ((wordEntry.flags & bit) != 0)
          {
            if (wordEntry.flags != lastValue)
            {
              lastValue = wordEntry.flags;
              text.append ("\n");
            }
            if (wordEntry.flags == 0x80)            // nouns are all in one entry
            {
              for (WordEntry we : list)
                text.append (we + "\n");
            }
            else
              text.append (wordEntry + "\n");
          }
        }
        text.append ("\n");
      }
    }

    text.deleteCharAt (text.length () - 1);
    return text.toString ();
  }

  // ---------------------------------------------------------------------------------//
  private class WordEntry implements Comparable<WordEntry>
  // ---------------------------------------------------------------------------------//
  {
    ZString word;
    int seq;
    int flags;
    int key;
    String bits;

    public WordEntry (ZString word, int seq)
    {
      this.word = word;
      this.seq = seq;

      // build key from 3 bytes following the word characters
      int b1 = buffer[word.startPtr + 4] & 0xFF;
      int b2 = buffer[word.startPtr + 5] & 0xFF;
      int b3 = buffer[word.startPtr + 6] & 0xFF;

      this.key = (b1 << 16) | (b2 << 8) | b3;
      this.flags = b1;
      this.bits = Integer.toBinaryString (b1);
      if (bits.length () < 8)
        bits = "00000000".substring (bits.length ()) + bits;
    }

    @Override
    public int compareTo (WordEntry o)
    {
      return this.flags - o.flags;
    }

    @Override
    public String toString ()
    {
      StringBuilder list = new StringBuilder ("[");
      if ((key & 0x0800000) == 0)
      {
        for (WordEntry we : synonymList.get (key))
          list.append (we.word.value + ", ");
        list.deleteCharAt (list.length () - 1);
        list.deleteCharAt (list.length () - 1);
      }
      else
        list.append (word.value);
      list.append ("]");

      StringBuilder text = new StringBuilder ();
      text.append (String.format ("%04X   %3d   %-6s   %s  %s   %s", word.startPtr, seq,
          word.value, bits,
          HexFormatter.getHexString (buffer, word.startPtr + 4, entryLength - 4),
          list.toString ()));
      return text.toString ();
    }
  }
}