using Microsoft.Win32;
using Microsoft.Windows.Themes;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace A2DiskUtil.ViewModel
{
   /// <summary>
   /// ViewModel for the main window
   /// </summary>
   internal class MainViewModel : ViewModelBase
   {
      #region Public Properties

      /// <summary>
      /// Gets the DiskImage
      /// </summary>
      public DiskImage? DiskImage
      {
         get;
         private set;
      }

      /// <summary>
      /// Gets the Files collection
      /// </summary>
      public ObservableCollection<FileDescriptiveEntry> Files
      {
         get;
      } = new ObservableCollection<FileDescriptiveEntry>();

      #endregion

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

               Files.Clear();
               foreach (var file in DiskImage.GetCatalog())
                  Files.Add(file);
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
