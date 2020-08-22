using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Groups;
using Suna.JS;

namespace Suna.Link
{
   /// <summary>
   /// A context representing the current state in which linking is taking place
   /// </summary>
   public class LinkContext
   {
      private enum Error
      {
         IdentifierIsNotInvokable
      }

      private JsModule jsModule;

      /// <summary>
      /// Initializes a new instance of class LickContext
      /// </summary>
      /// <param name="sourceModule"></param>
      /// <param name="jsModule"></param>
      public LinkContext(BlockifiedModule sourceModule, JsModule jsModule)
      {
         this.SourceModule = sourceModule;
         this.jsModule = jsModule;
      }

      /// <summary>
      /// Gets the source module associated with the context
      /// </summary>
      public BlockifiedModule SourceModule
      {
         get;
         private set;
      }

      /// <summary>
      /// Gets the associated LinkedModule
      /// </summary>
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
         Contract.Requires(callParameters != null);

         // find the block
         Block invokableBlock = SourceModule.GetBlock(identifier);
         if (invokableBlock is InlineBlock)
         {
            ((InlineBlock)invokableBlock).Compile(this, callParameters);
         }
         else if (jsModule.IsFunctionName(identifier))
         {
            byte[] generatedCode = jsModule.EvaluateByteArray(identifier + callParameters.ToString());
            this.LinkedModule.AppendCode(generatedCode);
         }
         else
         {
            throw new CompileException(Error.IdentifierIsNotInvokable);
         }
      }
   }
}
