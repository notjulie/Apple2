using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AppleDev.Blocks;

namespace AppleDev.Tokens
{
   class KeywordToken : Token
   {
      #region Constructor

      /// <summary>
      /// Initializes a new instance of class KeywordToken
      /// </summary>
      /// <param name="keyword"></param>
      public KeywordToken(Keyword keyword)
         :base(new TokenInfo(keyword.ToString()))
      {
         this.Keyword = keyword;
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets the keyword
      /// </summary>
      public Keyword Keyword
      {
         get;
         private set;
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Creates a block of the type associated with this keyword
      /// </summary>
      /// <param name="tokens"></param>
      /// <returns></returns>
      public override Block CreateBlock(Token[] tokens)
      {
         switch (Keyword)
         {
            case Keyword.function:
               return new FunctionBlock(tokens);

            default:
               return base.CreateBlock(tokens);
         }
      }

      #endregion
   }
}
