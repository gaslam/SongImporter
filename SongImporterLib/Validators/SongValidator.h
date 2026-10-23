#ifndef SONGVALIDATOR_H
#define SONGVALIDATOR_H
#include <QValidator>

/// Validates any value based on what it is.
/// Via inheritance, many classes inherit from this base class
/// Each derived class can thus implement it's own validation.
/// I want to display error messages based on the input.
/// void validat is always const. So I had to do some tricks.
/// QValidator is needed for the input in qml to validate the state of the input.

class SONGIMPORTERLIB_EXPORT SongValidator : public QValidator
{
	Q_OBJECT
	Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
	using State = QValidator::State;
	SongValidator(QObject* parent = nullptr) : QValidator(parent) {}
	virtual ~SongValidator() = default;

	// Inherited via QValidator
    /// Validates a value.
    /// \param value The value to be validated.
    /// \param pos The position of the text cursor
    /// \return An acceptable or intermediate state
	virtual State validate(QString&, int&) const override = 0;
	[[nodiscard]] QString errorMessage() const { return m_ErrorMessage; }

protected:

	void setErrorMessage(const QString& message) const {
		if (m_ErrorMessage == message)
		{
			return;
		}
		m_ErrorMessage = message;
		emit errorMessageChanged(m_ErrorMessage);
	}

	void clearErrorMessage() const {
		if (m_ErrorMessage.isEmpty())
		{
			return;
		}
		m_ErrorMessage.clear();
		emit errorMessageChanged(m_ErrorMessage);
	}

signals:
	void errorMessageChanged(const QString& errorMessage) const;

private:
    /// This doesn't change the logical state of the object. Of course,
    /// we must try to avoid changing it in const functions, so any other suggestions are welcome.
	mutable QString m_ErrorMessage;
};
#endif
