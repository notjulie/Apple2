using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Groups;
using Suna.Link;
using Suna.Tokens;

namespace Suna.Blocks
{
   class InlineBlock : Block
   {
      public InlineBlock(Token[] tokens)
         :base(tokens)
      {
         Name = ((IdentifierToken)tokens[1]).Identifier;
      }

      public string Name
      {
         get;
         private set;
      }

      public void Compile(LinkContext context, ParentheticGroup callParameters)
      {
         throw new NotImplementedException();
      }
   }
}
