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
      private IEnumerator<Token> lastTokens;

      public GroupedBlock GroupifyTokens(IEnumerator<Token> tokens)
      {
         // dummy to prevent warnings for the moment
         lastTokens = tokens;

         GroupedBlock result = new GroupedBlock();
         for (; ;)
         {
            // if we're done, all good
            if (!tokens.MoveNext())
               return result;

            // let the token itself build the group
            result.Add(tokens.Current.ReadGroup(tokens));
         }

         throw new NotImplementedException();
      }
   }
}
