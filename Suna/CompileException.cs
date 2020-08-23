using Suna.Tokens;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Suna
{
   /// <summary>
   /// Exception during compiling
   /// </summary>
   [Serializable]
   public class CompileException : Exception
   {
      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      public CompileException()
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      public CompileException(Enum message)
         : this(FormatMessage(message))
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      public CompileException(string message)
         : base(message)
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      /// <param name="inner"></param>
      public CompileException(string message, Exception inner)
         : base(message, inner)
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      /// <param name="inner"></param>
      public CompileException(Enum message, Exception inner)
         : this(FormatMessage(message), inner)
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="message"></param>
      /// <param name="token"></param>
      public CompileException(Enum message, Token token)
         : this(FormatMessage(message, token))
      {
      }

      /// <summary>
      /// Initializes a new instance of class CompileException
      /// </summary>
      /// <param name="info"></param>
      /// <param name="context"></param>
      protected CompileException(SerializationInfo info, StreamingContext context)
         : base(info, context)
      {
      }

      private static string FormatMessage(Enum error)
      {
         return error.ToString();
      }

      private static string FormatMessage(Enum error, Token token)
      {
         return error.ToString() + ": " + token;
      }
   }
}
