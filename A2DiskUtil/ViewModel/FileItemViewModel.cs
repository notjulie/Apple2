using A2DiskUtil.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace A2DiskUtil.ViewModel
{
   public class FileItemViewModel : ViewModelBase
   {
      private FileDescriptiveEntry fileDescriptiveEntry;

      public FileItemViewModel(FileDescriptiveEntry fileDescriptiveEntry)
      {
         this.fileDescriptiveEntry = fileDescriptiveEntry;
      }

      public override string ToString()
      {
         return fileDescriptiveEntry.ToString();
      }
   }
}
