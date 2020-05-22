using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Groups;

namespace Suna.Link
{
   class LinkContext
   {
      private enum Error
      {
         IdentifierIsNotInvokable
      }

      private Jint.Engine jsEngine;

      public LinkContext(BlockifiedModule sourceModule, Jint.Engine jsEngine)
      {
         this.SourceModule = sourceModule;
         this.jsEngine = jsEngine;
      }

      public BlockifiedModule SourceModule
      {
         get;
         private set;
      }

      public LinkedModule LinkedModule
      {
         get;
      } = new LinkedModule();

      /// <summary>
      /// Compiles the invocation of the invokable item with the given name, using the
      /// given parameters.
      /// </summary>
      /// <param name="identifier">the name of the invokable item</param>
      /// <param name="callParameters">the parameters</param>
      public void CompileInvocation(string identifier, ParentheticGroup callParameters)
      {
         // find the block
         Block invokableBlock = SourceModule.GetBlock(identifier);
         if (invokableBlock is InlineBlock)
         {
            ((InlineBlock)invokableBlock).Compile(this, callParameters);
         }
         else
         {
            throw new CompileException(Error.IdentifierIsNotInvokable);
         }
      }
   }
}
