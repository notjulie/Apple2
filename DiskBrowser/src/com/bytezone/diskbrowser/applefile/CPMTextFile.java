package com.bytezone.diskbrowser.applefile;

// -----------------------------------------------------------------------------------//
public class CPMTextFile extends TextFile
// -----------------------------------------------------------------------------------//
{
  // ---------------------------------------------------------------------------------//
  public CPMTextFile (String name, byte[] buffer)
  // ---------------------------------------------------------------------------------//
  {
    super (name, buffer);
  }

  // ---------------------------------------------------------------------------------//
  @Override
  public String getText ()
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder text = new StringBuilder ();

    if (textPreferences.showHeader)
      text.append ("Name : " + name + "\n\n");

    int ptr = 0;
    while (ptr < buffer.length && buffer[ptr] != (byte) 0x1A)
    {
      String line = getLine (ptr);
      text.append (line + "\n");
      ptr += line.length () + 1;
      if (ptr < buffer.length && buffer[ptr - 1] == 0x0D && buffer[ptr] == 0x0A)
        ++ptr;

      while (ptr < buffer.length && buffer[ptr] == 0)
        ++ptr;
    }

    if (text.length () > 0)
      text.deleteCharAt (text.length () - 1);

    return text.toString ();
  }

  // ---------------------------------------------------------------------------------//
  private String getLine (int ptr)
  // ---------------------------------------------------------------------------------//
  {
    StringBuilder line = new StringBuilder ();

//    int max = buffer.length - 1;
    while (ptr < buffer.length && buffer[ptr] != 0x0D && buffer[ptr] != 0x0A)
      line.append ((char) (buffer[ptr++] & 0x7F));

    return line.toString ();
  }
}