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
      #region Private Fields

      private DiskImage? diskImage;
      private ObservableCollection<FileItemViewModel> files = new ObservableCollection<FileItemViewModel>();

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class MainViewModel
      /// </summary>
      public MainViewModel()
      {
         // expose our read-only version of the files collection
         Files = new ReadOnlyObservableCollection<FileItemViewModel>(files);
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the DiskImage
      /// </summary>
      public DiskImage? DiskImage
      {
         get
         {
            return diskImage;
         }
         private set
         {
            // accept the value
            diskImage = value;

            // update our files list
            files.Clear();
            if (diskImage != null) 
            {
               foreach (var file in diskImage.GetCatalog())
                  if (file.IsFile)
                     files.Add(new FileItemViewModel(this, file));
            }
         }
      }

      /// <summary>
      /// Gets the Files collection
      /// </summary>
      public ReadOnlyObservableCollection<FileItemViewModel> Files
      {
         get;
         private set;
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Deletes the given file
      /// </summary>
      /// <param name="file"></param>
      public void DeleteFile(A2FileName file)
      {
         diskImage.DeleteFile(file);
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
            }
            catch (Exception ex)
            {
               MessageBox.Show(ex.Message);
            }
         }
      }

      /// <summary>
      /// Prompts the user to save the current disk to a file
      /// </summary>
      public void SaveDiskAs()
      {
         SaveFileDialog dialog = new SaveFileDialog();
         if (dialog.ShowDialog() == true)
         {
            try
            {
               DiskImage.SaveAs(dialog.FileName);
            }
            catch (Exception ex)
            {
               MessageBox.Show(ex.Message);
            }
         }
      }

      #endregion

      #region Protected Methods

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
            LoadA2File(filename);
            return;
         }

         // anything else must be accepted by the base class
         base.DropFile(filename);
      }

      #endregion

      #region Private Methods

      /// <summary>
      /// Loads an A2 file and writes it to our disk image
      /// </summary>
      /// <param name="filename"></param>
      private void LoadA2File(string filename)
      {
         try
         {
            // load the file
            A2File a2File = new A2File(filename);

            // make a working copy of our image
            if (DiskImage == null)
               throw new Exception("A valid Disk Image has not been loaded");
            DiskImage diskImageCopy = DiskImage.Clone();

            // write the file
            diskImageCopy.WriteFile(a2File);

            // all's well, accept the new image
            DiskImage = diskImageCopy;
         }
         catch (Exception e)
         {
            MessageBox.Show(e.Message);
         }
      }

      #endregion
   }
}
