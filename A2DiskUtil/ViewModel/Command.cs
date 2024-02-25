using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace A2DiskUtil.ViewModel
{
   public class Command : ICommand
   {
      #region Types / Constants

      public delegate void Executor(object? parameter);

      #endregion

      #region Private Fields

      private bool canExecute = true;
      private event EventHandler? canExecuteChanged;
      private Executor executor;

      #endregion

      #region Constructor

      public Command(Executor executor)
      {
         this.executor = executor;
      }

      #endregion

      #region ICommand implementation

      event EventHandler? ICommand.CanExecuteChanged
      {
         add
         {
            canExecuteChanged += value;
         }

         remove
         {
            canExecuteChanged -= value;
         }
      }

      bool ICommand.CanExecute(object? parameter)
      {
         return canExecute;
      }

      void ICommand.Execute(object? parameter)
      {
         executor(parameter);
      }

      #endregion
   }
}
