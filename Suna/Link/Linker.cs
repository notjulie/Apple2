using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;

namespace Suna.Link
{
   /// <summary>
   /// Linker class; it's job is to take a module and convert it to code.  Compilation
   /// is done as-needed, so we get called from the top level of the process and invoke
   /// compilation as necessary.
   /// </summary>
   class Linker
   {
      #region Types / Constants

      private enum Error
      {
         NoMain
      }

      #endregion

      #region Private Fields

      private BlockifiedModule module;

      #endregion

      #region Public Methods

      /// <summary>
      /// Links the given module
      /// </summary>
      /// <param name="module"></param>
      /// <returns></returns>
      public LinkedModule Link(BlockifiedModule module)
      {
         // save parameters
         this.module = module;

         // create the result
         LinkedModule result = new LinkedModule();

         // find the "main" block
         MainBlock main = module.Main;
         if (main == null)
            throw new CompileException(Error.NoMain);

         // create the base (global) context
         LinkContext context = new LinkContext(module, result);

         // let main compile itself in the global context
         main.Compile(context);

         // now what?
         throw new NotImplementedException();
      }

      #endregion
   }
}
