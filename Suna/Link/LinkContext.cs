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
      private List<BlockifiedModule> modules = new List<BlockifiedModule>();

      /// <summary>
      /// Initializes a new instance of class LickContext
      /// </summary>
      public LinkContext()
      {
      }

      /// <summary>
      /// Gets the associated LinkedModule
      /// </summary>
      public LinkedModule LinkedModule
      {
         get;
      } = new LinkedModule();

      /// <summary>
      /// Gets the main block
      /// </summary>
      public MainBlock Main
      {
         get
         {
            return modules[0].Main;
         }
      }

      /// <summary>
      /// Adds a javaScript region
      /// </summary>
      public void AddJavaScriptRegion(JavascriptRegion javaScriptRegion)
      {
         Contract.Requires(javaScriptRegion != null);

         jsModule.Execute(javaScriptRegion.ToString());
      }

      /// <summary>
      /// Adds the given module
      /// </summary>
      /// <param name="module"></param>
      public void AddModule(BlockifiedModule module)
      {
         modules.Add(module);
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
         Block invokableBlock = GetBlock(identifier);
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

      private Block GetBlock(IdentifierToken identifier)
      {
         foreach (var module in this.modules)
         {
            var block = module.GetBlock(identifier.Identifier);
            if (block != null)
               return block;
         }

         return null;
      }
   }
}
