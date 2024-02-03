using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace A2DiskUtil
{
   internal class ViewModelBase : INotifyPropertyChanged
   {
      public event PropertyChangedEventHandler? PropertyChanged;

      protected virtual void OnPropertyChanged(string propertyName)
      {
         PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
      }
   }
}