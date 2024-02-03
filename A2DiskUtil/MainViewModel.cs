using Microsoft.Win32;
using Microsoft.Windows.Themes;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace A2DiskUtil
{
   internal class MainViewModel : ViewModelBase
   {
      /// <summary>
      /// Gets the DiskImage
      /// </summary>
      public DiskImage DiskImage
      {
         get;
         private set;
      }

      #region Public Methods

      /// <summary>
      /// Prompts the user to open a disk image file
      /// </summary>
      public void OpenFile()
      {
         OpenFileDialog dialog = new OpenFileDialog();
         if (dialog.ShowDialog() == true)
         {
            try
            {
               DiskImage = new DiskImage(dialog.FileName);
            }
            catch (Exception ex)
            {
               MessageBox.Show(ex.Message);
            }
         }
      }

      #endregion
   }
}
