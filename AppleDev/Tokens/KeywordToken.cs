﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   class KeywordToken : Token
   {
      public KeywordToken(Keyword keyword)
         :base(new TokenInfo(keyword.ToString()))
      {
         this.Keyword = keyword;
      }

      public Keyword Keyword
      {
         get;
         private set;
      }
   }
}
