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
   public class FileItemViewModel(MainViewModel parent, FileDescriptiveEntry fileDescriptiveEntry) : ViewModelBase
   {
      #region Public Properties

      public ICommand DeleteFile
      {
         get;
         private set;
      } = new Command((object? o) => {
            try
            {
               parent.DeleteFile(fileDescriptiveEntry.FileName);
            }
            catch (Exception e)
            {
               MessageBox.Show(e.Message);
            }
         });

      #endregion

      #region Base class overrides

      public override string ToString()
      {
         return fileDescriptiveEntry.ToString();
      }

      #endregion
   }
}
