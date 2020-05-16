using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Tokens;

namespace Suna.Groups
{
   class Groupifier
   {
      // dummy to prevent warnings for the moment
      private Token[] lastTokens;

      public GroupedBlock GroupifyTokens(Token[] tokens)
      {
         // dummy to prevent warnings for the moment
         lastTokens = tokens;

         GroupedBlock result = new GroupedBlock();
         throw new NotImplementedException();
      }
   }
}
