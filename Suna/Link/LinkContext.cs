using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
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
      private List<ImportBlock> imports = new List<ImportBlock>();
      private List<ImportBlock> loadedImports = new List<ImportBlock>();

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
         Contract.Requires(module != null);

         // add it to our list
         modules.Add(module);

         // add its imports
         foreach (var import in module.Imports)
            imports.Add(import);

         // load any imports that haven't already been loaded
         LoadImports();
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

      private void LoadImports()
      {
         foreach (var import in imports)
         {
            // see if it's already loaded
            bool isLoaded = false;
            foreach (var loadedImport in loadedImports)
            {
               string path1 = Path.GetFullPath(import.Path).ToUpperInvariant();
               string path2 = Path.GetFullPath(loadedImport.Path).ToUpperInvariant();
               if (path1 == path2)
                  isLoaded = true;
            }

            if (!isLoaded)
               jsModule.Execute(import.LoadString());
         }
      }
   }
}
