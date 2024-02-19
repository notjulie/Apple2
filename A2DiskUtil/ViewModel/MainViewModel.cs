using A2DiskUtil.Model;
using Microsoft.Win32;
using Microsoft.Windows.Themes;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.IO;
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
    public class MainViewModel : ViewModelBase
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
      /// Returns a value indicating whether we allow dropping the given file
      /// </summary>
      /// <param name="fileName"></param>
      /// <returns></returns>
      protected override bool CanDropFile(string filename)
      {
         if (Path.GetExtension(filename).ToLower() == ".a2")
            return true;
         else
            return base.CanDropFile(filename);
      }

      /// <summary>
      /// Processes a dropped file
      /// </summary>
      /// <param name="filename"></param>
      protected override void DropFile(string filename)
      {
         // if it's an a2 file (my own type of Apple2 binary) handle that
         if (Path.GetExtension(filename).ToLower() == ".a2")
         {
            return;
         }

         // anything else must be accepted by the base class
         base.DropFile(filename);
      }

      /// <summary>
      /// Prompts the user to open a disk image file
      /// </summary>
      public void OpenDiskImageFile()
      {
         OpenFileDialog dialog = new OpenFileDialog();
         if (dialog.ShowDialog() == true)
         {
            try
            {
               DiskImage = new DiskImage(dialog.FileName);

               Files.Clear();
               foreach (var file in DiskImage.GetCatalog())
                  if (file.IsFile)
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
