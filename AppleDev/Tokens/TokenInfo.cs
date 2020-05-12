using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   struct TokenInfo
   {
      public TokenInfo(string text)
      {
         this.Text = text;
      }

      public string Text
      {
         get;
         private set;
      }
   }
}
