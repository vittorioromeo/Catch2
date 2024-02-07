
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_ASSERTION_HANDLER_HPP_INCLUDED
#define CATCH_ASSERTION_HANDLER_HPP_INCLUDED

#include <catch2/catch_assertion_info.hpp>
#include <catch2/internal/catch_stringrefbase.hpp>

namespace Catch {

    class ITransientExpression;
    class IPrintableExpression;
    class IResultCapture;

    struct AssertionReaction {
        bool shouldDebugBreak = false;
        bool shouldThrow = false;
        bool shouldSkip = false;
    };

    class AssertionHandler {
        AssertionInfo m_assertionInfo;
        AssertionReaction m_reaction;
        bool m_completed = false;
        IResultCapture& m_resultCapture;

    public:
        AssertionHandler
            (   StringRefBase macroName,
                SourceLineInfo const& lineInfo,
                StringRefBase capturedExpression,
                ResultDisposition::Flags resultDisposition );
        ~AssertionHandler();


        /*template<typename T>
        void handleExpr( ExprLhs<T> const& expr ) {
            handleExpr( expr.makeUnaryExpr() );
        }*/
        void handleExpr( IPrintableExpression const& expr );
        void handleExpr( ITransientExpression const& expr );

        void handleMessage(ResultWas::OfType resultType, StringRefBase message);

        void handleExceptionThrownAsExpected();
        void handleUnexpectedExceptionNotThrown();
        void handleExceptionNotThrownAsExpected();
        void handleThrowingCallSkipped();
        void handleUnexpectedInflightException();

        void complete();

        // query
        auto allowThrows() const -> bool;
    };

    void handleExceptionMatchExpr( AssertionHandler& handler, StringRefBase str );

} // namespace Catch

#endif // CATCH_ASSERTION_HANDLER_HPP_INCLUDED
