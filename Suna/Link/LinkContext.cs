using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Groups;
using Suna.JS;
using Suna.Regions;
using Suna.Tokens;

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

      private JsModule jsModule = new JsModule();

      /// <summary>
      /// Initializes a new instance of class LickContext
      /// </summary>
      /// <param name="sourceModule"></param>
      public LinkContext(BlockifiedModule sourceModule)
      {
         this.SourceModule = sourceModule;
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
      /// Adds a javaScript region
      /// </summary>
      public void AddJavaScriptRegion(JavascriptRegion javaScriptRegion)
      {
         Contract.Requires(javaScriptRegion != null);

         jsModule.Execute(javaScriptRegion.ToString());
      }

      /// <summary>
      /// Compiles the invocation of the invokable item with the given name, using the
      /// given parameters.
      /// </summary>
      /// <param name="identifier">the name of the invokable item</param>
      /// <param name="callParameters">the parameters</param>
      public void CompileInvocation(IdentifierToken identifier, ParentheticGroup callParameters)
      {
         Contract.Requires(callParameters != null);
         Contract.Requires(identifier != null);

         // find the block
         Block invokableBlock = SourceModule.GetBlock(identifier.Identifier);
         if (invokableBlock is InlineBlock)
         {
            ((InlineBlock)invokableBlock).Compile(this, callParameters);
         }
         else if (jsModule.IsFunctionName(identifier.Identifier))
         {
            byte[] generatedCode = jsModule.EvaluateByteArray(identifier + callParameters.ToString());
            this.LinkedModule.AppendCode(generatedCode);
         }
         else
         {
            throw new CompileException(Error.IdentifierIsNotInvokable, identifier);
         }
      }
   }
}
