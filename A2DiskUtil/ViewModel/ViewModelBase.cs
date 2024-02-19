using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;

namespace A2DiskUtil.ViewModel
{
   /// <summary>
   /// Base class for our application's ViewModel classes
   /// </summary>
   public class ViewModelBase : INotifyPropertyChanged
   {
      #region Public Events

      public event PropertyChangedEventHandler? PropertyChanged;

      #endregion

      #region Public Methods
      
      /// <summary>
      /// Examines the data object to decide if we can accept it
      /// </summary>
      /// <param name="dataObject"></param>
      /// <returns></returns>
      public bool CanDropData(IDataObject dataObject)
      {
         // if it's a file see if we accept it
         string filename = GetObjectAsFilename(dataObject);
         if (filename != null)
            return CanDropFile(filename);

         // anything else we don't do
         return false;
      }

      public void DropData(IDataObject dataObject)
      {
         string filename = GetObjectAsFilename(dataObject);
         if (filename != null)
         {
            DropFile(filename);
            return;
         }
      }

      #endregion

      #region Protected Methods

      /// <summary>
      /// Returns true if we would accept the named file as dropped data
      /// </summary>
      /// <param name="filename"></param>
      /// <returns></returns>
      protected virtual bool CanDropFile(string filename)
      {
         return false;
      }

      /// <summary>
      /// Overridden by child classes to handle file drop events
      /// </summary>
      /// <param name="filename"></param>
      protected virtual void DropFile(string filename)
      {
      }

      #endregion

      #region Event Handlers

      protected virtual void OnPropertyChanged(string propertyName)
      {
         PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
      }

      #endregion

      #region Private Methods

      /// <summary>
      /// Gets the filename associated with the given data object; returns null if the object
      /// is not a file
      /// </summary>
      /// <param name="dataObject"></param>
      /// <returns></returns>
      private string GetObjectAsFilename(IDataObject dataObject)
      {
         bool isFile = dataObject.GetFormats().Contains("FileName");
         if (isFile)
         {
            string[]? data = dataObject.GetData("FileName") as string[];
            if (data != null && data.Length > 0)
            {
               return data[0];
            }
         }

         return null;
      }

      #endregion
   }
}