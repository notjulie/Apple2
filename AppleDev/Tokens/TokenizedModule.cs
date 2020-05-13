using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AppleDev.Tokens
{
   class TokenizedModule : IEnumerable<Token>
   {
      #region Private Fields

      private List<Token> tokens = new List<Token>();

      #endregion

      #region Public Methods

      public void Add(Token token)
      {
         tokens.Add(token);
      }

      #endregion

      #region IEnumerable Implementation

      public IEnumerator<Token> GetEnumerator()
      {
         throw new NotImplementedException();
      }

      IEnumerator IEnumerable.GetEnumerator()
      {
         throw new NotImplementedException();
      }

      #endregion
   }
}
