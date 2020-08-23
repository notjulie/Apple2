using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Blocks
{
   /// <summary>
   /// An import of an external module
   /// </summary>
   public class ImportBlock : Block
   {
      private string path = string.Empty;

      /// <summary>
      /// Initializes a new instance of class ImportBlock
      /// </summary>
      /// <param name="tokens"></param>
      public ImportBlock(Token[] tokens)
         : base(tokens)
      {
         Contract.Requires(tokens != null);

         path = ((StringToken)tokens[1]).Value;
      }

      /// <summary>
      /// Gets the path
      /// </summary>
      public string Path
      {
         get
         {
            // for now just assume that it is relative to this assembly
            Assembly assembly = Assembly.GetExecutingAssembly();
            var assemblyFile = assembly.Location;
            var assemblyDir = System.IO.Path.GetDirectoryName(assemblyFile);
            return System.IO.Path.Combine(assemblyDir, this.path);
         }
      }

      /// <summary>
      /// Adds this instance to the given module
      /// </summary>
      /// <param name="blockifiedModule"></param>
      public override void AddToModule(BlockifiedModule blockifiedModule)
      {
         Contract.Requires(blockifiedModule != null);

         blockifiedModule.AddImport(this);
      }

      /// <summary>
      /// Loads the import as a string
      /// </summary>
      /// <returns></returns>
      public string LoadString()
      {
         using (var reader = new StreamReader(Path))
            return reader.ReadToEnd();
      }
   }
}
