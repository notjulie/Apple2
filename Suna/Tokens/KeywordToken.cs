using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;

namespace Suna.Tokens
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
      /// Gets a value indicating whether this token indicates the start of a blocks
      /// </summary>
      public override bool IsBlockStart
      {
         get
         {
            switch (Keyword)
            {
               case Keyword.function:
               case Keyword.inline:
                  return true;

               default:
                  return false;
            }
         }
      }

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

            case Keyword.inline:
               return new InlineBlock(tokens);

            default:
               return base.CreateBlock(tokens);
         }
      }

      #endregion
   }
}
