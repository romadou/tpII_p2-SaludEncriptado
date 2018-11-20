class Measure < ApplicationRecord
  belongs_to :patient
  belongs_to :sensor

  validates :value, :patient, :sensor, presence: true

  def decrypted_value
    # El valor llega codificado en Base64
    decoded = Base64.decode64(self[:value])

    c = OpenSSL::Cipher::Cipher.new("aes-128-cbc")
    # Objeto de cifrado en modo para desencriptar
    c.decrypt
    # Importante, puesto que la información viene sin padding
    c.padding = 0
    # La clave y el vector de inicialización deben ser los mismos que los puestos al encriptar
    c.key = 'aaaabbbbccccdddd'
    c.iv = 'ABCDEFGHIJKLMNOP'
    d = c.update(decoded).unpack("A*").first.to_f

  end
end
