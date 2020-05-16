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
   /// Class representing the block indicated by the "main" keyword
   /// </summary>
   class MainBlock  : Block
   {
      /// <summary>
      /// Initializes a new instance of class MainBlock
      /// </summary>
      /// <param name="tokens"></param>
      public MainBlock(Token[] tokens)
         :base(tokens)
      {
      }

      /// <summary>
      /// Compiles the block
      /// </summary>
      /// <param name="linkContext"></param>
      public void Compile(LinkContext linkContext)
      {
         Contract.Requires(linkContext != null);

         // get an enumerator and move to the first item
         var enumerator = this.Tokens.GetEnumerator();

         // skip the "main" keyword
         enumerator.MoveNext();

         // groupify the contents of the block... the block's contents start immediately
         // after the main keyword
         GroupItem groupedBlock = Groupifier.GroupifyTokens(enumerator);

         throw new NotImplementedException();
      }
   }
}
