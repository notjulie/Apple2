using A2DiskUtil.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;

namespace A2DiskUtil.ViewModel
{
   /// <summary>
   /// View model for a file item
   /// </summary>
   public class FileItemViewModel : ViewModelBase
   {
      #region Private Fields

      private MainViewModel parent;
      private FileDescriptiveEntry fileDescriptiveEntry;

      #endregion

      #region Constructor

      public FileItemViewModel(MainViewModel parent, FileDescriptiveEntry fileDescriptiveEntry)
      {
         // copy parameters
         this.parent = parent;
         this.fileDescriptiveEntry = fileDescriptiveEntry;

         // initialize other stuff
         this.DeleteFile = new Command((object? o) => {
            try
            {
               parent.DeleteFile(fileDescriptiveEntry.FileName);
            }
            catch (Exception e)
            {
               MessageBox.Show(e.Message);
            }
         });
      }

      #endregion

      #region Public Properties

      public ICommand DeleteFile
      {
         get;
         private set;
      }

      #endregion

      #region Base class overrides

      public override string ToString()
      {
         return fileDescriptiveEntry.ToString();
      }

      #endregion
   }
}
