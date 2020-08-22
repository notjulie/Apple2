using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.Groups;
using Suna.Link;

namespace Suna.Tokens
{
   /// <summary>
   /// Base class representing a token in the source
   /// </summary>
   public class Token
   {
      #region Types / Constants

      private enum Error
      {
         InternalErrorEmptyToken,
         InternalErrorInvalidBlockHeaderToken,
         TokenCannotBeCompiled
      }

      #endregion

      #region Constructor

      /// <summary>
      /// Initializes a new instance of class Token
      /// </summary>
      /// <param name="tokenInfo"></param>
      public Token(TokenInfo tokenInfo)
      {
         this.TokenInfo = tokenInfo;

         if (TokenInfo.Text.Length == 0)
            throw new CompileException(Error.InternalErrorEmptyToken);
      }

      #endregion

      #region Public Properties

      /// <summary>
      /// Gets a value indicating whether the token is the start of a block
      /// </summary>
      virtual public bool IsBlockStart
      {
         get;
      } = false;

      /// <summary>
      /// Gets information about the token
      /// </summary>
      public TokenInfo TokenInfo
      {
         get;
         private set;
      }

      #endregion

      #region Public Methods

      /// <summary>
      /// Creates a Block from the given list of tokens; this is called on the block-header
      /// token (e.g. "function" keyword), so that the correct type of block can be created.
      /// Blocks that are not block-start tokens should just call the base implementation,
      /// i.e. they should throw the error that we throw below.
      /// </summary>
      /// <param name="tokens"></param>
      /// <returns></returns>
      public virtual Block CreateBlock(Token[] tokens)
      {
         throw new CompileException(Error.InternalErrorInvalidBlockHeaderToken);
      }

      /// <summary>
      /// Pulls the next GroupItem from the token stream
      /// </summary>
      /// <param name="enumerator"></param>
      /// <returns></returns>
      public virtual GroupItem ReadGroupItem(IEnumerator<Token> enumerator)
      {
         return new TokenGroupItem(this);
      }

      /// <summary>
      /// Compiles the token; in general a single token doesn't mean enough by
      /// itself to be compilable, but give them the option
      /// </summary>
      /// <param name="linkContext"></param>
      public virtual void Compile(LinkContext linkContext)
      {
         throw new CompileException(Error.TokenCannotBeCompiled);
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Returns a string representation of the object.
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return TokenInfo.Text;
      }

      #endregion
   }
}
