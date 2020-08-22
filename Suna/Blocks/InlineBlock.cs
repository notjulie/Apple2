using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Groups;
using Suna.Link;
using Suna.Tokens;

namespace Suna.Blocks
{
   /// <summary>
   /// Represents a block in the code that starts with the "inline" keyword.  It's
   /// a function type syntax that is evaluated somewhat like a C macro expansion.
   /// </summary>
   public class InlineBlock : Block
   {
      /// <summary>
      /// Initializes a new instance of class InlineBlock
      /// </summary>
      /// <param name="tokens"></param>
      public InlineBlock(Token[] tokens)
         :base(tokens)
      {
         Contract.Requires(tokens != null);

         Name = ((IdentifierToken)tokens[1]).Identifier;
      }

      /// <summary>
      /// Gets the block's name
      /// </summary>
      public string Name
      {
         get;
         private set;
      }

      /// <summary>
      /// Compiles the block to the given context with the given parameters
      /// </summary>
      /// <param name="context"></param>
      /// <param name="callParameters"></param>
      public void Compile(LinkContext context, ParentheticGroup callParameters)
      {
         Contract.Requires(context != null);
         Contract.Requires(callParameters != null);

         // groupify the contents of the block
         Group groupedBlock = (Group)GroupedBlock.Read(this.Tokens.GetEnumerator());

         // there should be 4 items in the group:
         //   0: inline keyword
         //   1: identifier token
         //   2: parenthetical containing the parameter list
         //   3: code block

         // just compile the code for now
         groupedBlock.Items.ElementAt(3).Compile(context);
      }

      /// <summary>
      /// Adds this object to the given module
      /// </summary>
      /// <param name="blockifiedModule"></param>
      public override void AddToModule(BlockifiedModule blockifiedModule)
      {
         Contract.Requires(blockifiedModule != null);

         blockifiedModule.AddInline(this);
      }
   }
}
